#pragma once

#include "Array.h"
#include "Color.h"
#include "Files.h"
#include "Json.h"
#include "Logging.h"

#include "Interface/Style.h"
#include "Interface/Style.h"

namespace CSS
{
    enum class UnitType
    {
        None,
        Px,
        Em,
        Rem,
        Vw,
        Vh,
        Percent
    };

    enum class TokenType
    {
        End = 0,
        Keyword,
        Value,
        LBracket  = '{',
        RBracket  = '}',
        Semicolon = ';',
        Comma     = ',',
        Period    = '.',
        Slash     = '\\',
        Backslash = '/',
        Backquote = '\'',
        Quote     = '"',
        Colon     = ':',
        NewLine   = '\n'
    };

    struct Token
    {
        TokenType Type;
        std::string Value;
    };

    struct Number
    {
        float Value   = 0.0f;
        UnitType Unit = UnitType::None;

        std::string ToString() const
        {
            return std::format("{}", Value);
        }
    };

    struct Keyword
    {
        std::string Name;

        std::string ToString() const
        {
            return Name;
        }
    };

    using ValueType = std::variant<Keyword, Number, PColor, std::string>;

    struct Declaration
    {
        std::string Property;
        std::vector<ValueType> Values;

        JSON ToJson() const
        {
            JSON Result;

            Result[Property] = JSON::array();
            for (auto Value : Values)
            {
                if (std::holds_alternative<Keyword>(Value))
                {
                    Result[Property].push_back(std::get<Keyword>(Value).ToString());
                }
                else if (std::holds_alternative<Number>(Value))
                {
                    Result[Property].push_back(std::get<Number>(Value).ToString());
                }
                else if (std::holds_alternative<PColor>(Value))
                {
                    Result[Property].push_back(std::get<PColor>(Value).ToString());
                }
                else
                {
                    Result[Property].push_back(std::get<std::string>(Value));
                }
            }
            return Result;
        }
    };

    struct SimpleSelector
    {
        std::optional<std::string> Type;
        std::optional<std::string> Id;
        std::vector<std::string> Classes;
        std::vector<std::string> PseudoClasses;

        JSON ToJson() const
        {
            JSON Result;
            Result["Type"]          = Type.has_value() ? Type.value() : "null";
            Result["Id"]            = Id.has_value() ? Id.value() : "null";
            Result["Classes"]       = JSON::array();
            Result["PseudoClasses"] = JSON::array();
            for (auto Class : Classes)
            {
                Result["Classes"].push_back(Class);
            }
            for (auto Class : PseudoClasses)
            {
                Result["PseudoClasses"].push_back(Class);
            }
            return Result;
        }
    };

    struct Combinator
    {
        enum class Type { Descendant, Child, AdjacentSibling, GeneralSibling };
    };

    struct SelectorPart
    {
        std::optional<Combinator::Type> Leading;
        SimpleSelector Simple;

        JSON ToJson() const
        {
            JSON Result;
            Result["CombinatorType"] = Leading.has_value() ? std::to_string(static_cast<int>(Leading.value())) : "null";
            Result["Selector"]       = Simple.ToJson();
            return Result;
        }
    };

    struct Selector
    {
        std::vector<SelectorPart> Parts;

        JSON ToJson() const
        {
            JSON Result = JSON::array();
            for (auto Part : Parts)
            {
                Result.push_back(Part.ToJson());
            }
            return Result;
        }
    };

    struct Rule
    {
        std::vector<Selector> Selectors;
        std::vector<Declaration> Declarations;

        JSON ToJson() const
        {
            JSON Result;

            Result["Selectors"] = JSON::array();
            for (auto& Selector : Selectors)
            {
                Result["Selectors"].push_back(Selector.ToJson());
            }

            Result["Declarations"] = JSON::array();
            for (auto& Declaration : Declarations)
            {
                Result["Declarations"].push_back(Declaration.ToJson());
            }

            return Result;
        }
    };

    struct Stylesheet
    {
        std::vector<Rule> Rules;

        JSON ToJson() const
        {
            JSON Result = JSON::array();
            for (auto& Rule : Rules)
            {
                Result.push_back(Rule.ToJson());
            }
            return Result;
        }

        std::string ToString() const
        {
            return ToJson().dump(2);
        }
    };

    inline TArray<Token> Tokenize(const std::string& Buffer)
    {
        TArray<Token> Tokens;

        auto Ptr = Buffer.data();
        while (*Ptr != '\0')
        {
            switch (*Ptr)
            {
            default: break;
            }
        }
        return Tokens;
    }

    class Parser
    {
        const std::string mBuffer{};
        size_t mPos = 0;

        /* String stream */

        char Peek(size_t Length = 0)
        {
            size_t Index = mPos + Length;
            return Index < mBuffer.size() ? mBuffer[Index] : '\0';
        }

        char Next(size_t Length = 1)
        {
            mPos   = mPos + Length;
            auto C = mPos < mBuffer.size() ? mBuffer[mPos] : '\0';
            Skip();
            return C;
        }

        bool AtEnd() const
        {
            return mPos >= mBuffer.size();
        }

        char Get()
        {
            if (AtEnd())
            {
                return '\0';
            }
            return mBuffer[mPos++];
        }

        bool Match(char C)
        {
            if (!AtEnd() && Peek() == C)
            {
                mPos++;
                return true;
            }
            return false;
        }

        bool Expect(char C)
        {
            if (!AtEnd() && Peek() != C)
            {
                Error(std::format("Expected {}", C));
                return false;
            }
            Next();
            return true;
        }

        bool Skip()
        {
            bool Result = false;
            while (!AtEnd())
            {
                char C = Peek();
                if (C == ' ' || C == '\n' || C == '\r' || C == '\t')
                {
                    Result = true;
                    Next();
                    continue;
                }

                if (C == '/' && Peek(1) == '*')
                {
                    Result = true;
                    Next(2); // Skip '/*'
                    while (!AtEnd() && Peek() == '*' && Peek(1) != '/')
                    {
                        Next();
                    }
                    if (!AtEnd())
                    {
                        Next(2);
                    }
                    continue;
                }
                break;
            }
            return Result;
        }

        void Error(const std::string& Message)
        {
            LogError("{}: Pos:{}", Message.c_str(), mPos);
            mPos = std::string::npos;
        }

        /* Parsing */

        Rule ParseRule()
        {
            Rule Out;
            Out.Selectors = ParseSelectorGroup();

            if (!Expect('{'))
            {
                return Out;
            }

            while (!AtEnd() && Peek() != '}')
            {
                Out.Declarations.push_back(ParseDeclaration());
                Skip();
            }

            if (!Expect('}'))
            {
                return Out;
            }

            return Out;
        }

        std::vector<Selector> ParseSelectorGroup()
        {
            std::vector<Selector> Out;
            Out.push_back(ParseSelector());
            Skip();
            while (Match(','))
            {
                Skip();
                Out.push_back(ParseSelector());
                Skip();
            }
            return Out;
        }

        Selector ParseSelector()
        {
            Selector Out;
            std::optional<Combinator::Type> Comb;
            bool First = true;
            while (!AtEnd())
            {
                bool hadWS = Skip(); // true if we consumed whitespace
                // Determine combinator if not first or not immediately before '{' or ','
                if (!First)
                {
                    if (Match('>'))
                    {
                        Comb = Combinator::Type::Child;
                        Skip();
                    }
                    else if (Match('+'))
                    {
                        Comb = Combinator::Type::AdjacentSibling;
                        Skip();
                    }
                    else if (Match('~'))
                    {
                        Comb = Combinator::Type::GeneralSibling;
                        Skip();
                    }
                    else if (hadWS)
                    {
                        Comb = Combinator::Type::Descendant;
                    }
                }
                if (Peek() == '{' || Peek() == ',' || AtEnd())
                {
                    break;
                }
                SelectorPart part;
                part.Leading = First ? std::nullopt : Comb;
                part.Simple  = ParseSimpleSelector();
                Out.Parts.push_back(std::move(part));
                Comb.reset();
                First = false;
                if (Peek() == '{' || Peek() == ',' || AtEnd())
                {
                    break;
                }
            }
            if (Out.Parts.empty())
            {
                Error("Expected selector");
            }
            return Out;
        }

        SimpleSelector ParseSimpleSelector()
        {
            SimpleSelector s;
            // Type or universal
            if (Peek() == '*')
            {
                Get();
                s.Type = "*";
            }
            else if (IsIdentStart(Peek()))
            {
                s.Type = ParseIdent();
            }
            // .class, #id, :pseudo*
            while (!AtEnd())
            {
                if (Peek() == '.')
                {
                    Next();
                    s.Classes.push_back(RequireIdent("class name"));
                }
                else if (Peek() == '#')
                {
                    Get();
                    s.Id = RequireIdent("id");
                }
                else if (Peek() == ':')
                {
                    Get();
                    std::string pseudo = RequireIdent("pseudo-class");
                    if (Match('('))
                    {
                        pseudo.push_back('(');
                        pseudo += ParseUntilBalanced('(', ')');
                        if (!Expect(')'))
                        {
                            return s;
                        }
                        pseudo.push_back(')');
                    }
                    s.PseudoClasses.push_back(std::move(pseudo));
                }
                else
                {
                    break;
                }
            }
            if (!s.Type && s.Classes.empty() && !s.Id && s.PseudoClasses.empty())
            {
                Error("Expected a simple selector");
            }
            return s;
        }

        Declaration ParseDeclaration()
        {
            Declaration d;
            d.Property = RequireIdent("declaration property");
            Skip();
            if (!Expect(':'))
            {
                return d;
            }

            // values until ';' or '}' (not consuming '}')
            while (!AtEnd() && Peek() != ';' && Peek() != '}')
            {
                ValueType Value = ParseValueToken();
                d.Values.push_back(Value);
                bool had = Skip();
                // continue collecting tokens
            }
            if (Peek() == ';')
            {
                Get();
            }
            return d;
        }

        ValueType ParseValueToken()
        {
            char c = Peek();
            if (c == '#')
            { // hex color
                Get();
                std::string hex;
                while (std::isxdigit(Peek())) hex.push_back(Get());
                if (hex.empty())
                {
                    Error("Expected hex digits after '#'");
                }
                return PColor(hex);
            }
            if (std::isdigit(c) || (c == '-' && std::isdigit(Peek(1))))
            {
                Number num;
                num.Value = ParseNumber();
                // unit
                if (Match('%'))
                {
                    num.Unit = UnitType::Percent;
                }
                else if (IsIdentStart(Peek()))
                {
                    std::string u = ParseIdent();
                    if (u == "px") num.Unit = UnitType::Px;
                    else if (u == "em") num.Unit = UnitType::Em;
                    else if (u == "rem") num.Unit = UnitType::Rem;
                    else if (u == "vw") num.Unit = UnitType::Vw;
                    else if (u == "vh") num.Unit = UnitType::Vh;
                    else
                    { // unknown unit -> tuck back into raw text (as "value"+"unit")
                        // Represent as string to avoid losing info
                        std::ostringstream oss;
                        oss << num.Value << u;
                        return oss.str();
                    }
                }
                return num;
            }
            if (IsIdentStart(c))
            {
                std::string id = ParseIdent();
                // Common keywords recognized explicitly
                // but we keep the string as Keyword regardless.
                return Keyword{id};
            }
            if (c == '"' || c == '\'')
            {
                return ParseQuotedString();
            }
            // function token or fallback raw
            if (IsIdentStart(c))
            {
                std::string fn = ParseIdent();
                if (Match('('))
                {
                    std::string content = ParseUntilBalanced('(', ')');
                    if (!Expect(')'))
                    {
                        return "";
                    }
                    return fn + "(" + content + ")";
                }
            }
            // Otherwise, consume a single non-space, non-delimiter chunk as raw
            std::string raw;
            while (!AtEnd() && !std::isspace(Peek()) && Peek() != ';' && Peek() != '}')
            {
                raw.push_back(Get());
            }
            if (raw.empty())
            {
                Error("Unexpected token in value");
            }
            return raw;
        }

        bool IsIdentStart(char C)
        {
            return std::isalpha(C) || C == '_' || C == '-';
        }

        std::string RequireIdent(const char* what)
        {
            if (!IsIdentStart(Peek()))
            {
                Error(std::format("Expected {}, got {}", what, Peek()));
            }
            return ParseIdent();
        }

        std::string ParseIdent()
        {
            std::string Result;
            while (!AtEnd())
            {
                char C = Peek();
                if (std::isalpha(C) || C == '-' || C == '_')
                {
                    Result.push_back(C);
                    Next();
                }
                else
                {
                    break;
                }
            }
            return Result;
        }

        float ParseNumber()
        {
            std::string Result;
            if (Peek() == '+' || Peek() == '-')
            {
                Result.push_back(Get());
            }
            bool seenDot = false;
            while (!AtEnd())
            {
                char c = Peek();
                if (std::isdigit(static_cast<unsigned char>(c)))
                {
                    Result.push_back(Get());
                }
                else if (c == '.' && !seenDot)
                {
                    seenDot = true;
                    Result.push_back(Get());
                }
                else break;
            }
            if (Result == "+" || Result == "-" || Result.empty())
            {
                Error("Expected number");
            }
            return std::stof(Result);
        }

        ValueType ParseQuotedString()
        {
            char Quote = Get(); // ' or "
            std::string Result;
            while (!AtEnd())
            {
                char c = Get();
                if (c == Quote) break;
                if (c == '\\' && !AtEnd())
                {
                    char n = Get();
                    Result.push_back(n);
                }
                else
                {
                    Result.push_back(c);
                }
            }
            return Result;
        }

        std::string ParseUntilBalanced(char Open, char Close)
        {
            int depth = 1;
            std::string out;
            while (!AtEnd())
            {
                char c = Get();
                if (c == Close)
                {
                    --depth;
                    if (depth == 0)
                    {
                        // caller will consume the final ')'
                        // We already consumed the final ')', so step back one.
                        // Instead, return without including it and signal caller not to expect it.
                        // To keep logic simple: we *don't* include the final close, and we *do not* put it back.
                        // Caller already accounts for this by not reading ')' twice.
                        // Here, we return, and the caller will NOT call expect(')') because we consumed it.
                        // BUT elsewhere we coded caller to expect(')') — so we adjust:
                        // We'll include close into out and let caller consume its own ')'.
                        // To harmonize, add close to out and keep depth at 0, then break; caller still expects ')'.
                        out.push_back(c);
                        break;
                    }
                }
                else if (c == Open)
                {
                    ++depth;
                }
                out.push_back(c);
            }
            // Remove the trailing close we included to harmonize with caller:
            if (!out.empty() && out.back() == Close)
            {
                out.pop_back();
            }
            return out;
        }

    public:
        explicit Parser(const std::string& InBuffer)
            : mBuffer(InBuffer) {}

        Stylesheet Parse()
        {
            Stylesheet Sheet;

            Skip();
            while (!AtEnd())
            {
                if (AtEnd())
                {
                    break;
                }
                Sheet.Rules.push_back(ParseRule());
                Skip();
            }

            return Sheet;
        }
    };

    inline bool Load(Stylesheet* Sheet, const std::string& FileName)
    {
        auto File = Files::FindFile(FileName);
        if (File.empty())
        {
            LogError("CSS file {} not found.", FileName.c_str());
            return false;
        }

        std::string Buffer;
        if (!Files::ReadFile(File, Buffer))
        {
            LogError("CSS file {} could not be read.", FileName.c_str());
            return false;
        }

        *Sheet = Parser(Buffer).Parse();
        return true;
    }
}
