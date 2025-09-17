#pragma once

#include <cstdint>
#include <fstream>
#include <string.h>

constexpr size_t INT8_SIZE = sizeof(int8_t);
constexpr size_t INT16_SIZE = sizeof(int16_t);
constexpr size_t INT32_SIZE = sizeof(int32_t);
constexpr size_t INT64_SIZE = sizeof(int64_t);
constexpr size_t UINT8_SIZE = sizeof(uint8_t);
constexpr size_t UINT16_SIZE = sizeof(uint16_t);
constexpr size_t UINT32_SIZE = sizeof(uint32_t);
constexpr size_t UINT64_SIZE = sizeof(uint64_t);
constexpr size_t FLOAT_SIZE = sizeof(float);
constexpr size_t DOUBLE_SIZE = sizeof(double);

namespace Files
{
	using InStream = std::ifstream;
	using OutStream = std::ofstream;

	enum SeekMode
	{
		SM_Beginning = std::ios::beg,
		SM_Current = std::ios::cur,
		SM_End = std::ios::end
	};

	class BinaryWriter
	{
		OutStream Stream;
		size_t	  mPos = 0;
		size_t	  mSize = 0;

	public:
		BinaryWriter(const std::string& FileName)
		{
			Stream = OutStream(FileName, std::ios::binary | std::ios::out);
		}
		~BinaryWriter()
		{
			Close();
		}

		void   Flush() { Stream.flush(); }
		size_t Size() { return mSize; }
		void   Close() { Stream.close(); }
		size_t Pos() { return Stream.tellp(); }

		void Seek(size_t Offset, SeekMode Mode = SM_Current)
		{
			Stream.seekp(Offset, static_cast<std::ios_base::seekdir>(Mode));
		}

		void Write(const char* Data, size_t Offset, size_t Length)
		{
			if (Offset > 0)
			{
				Stream.seekp(Offset);
			}
			Stream.write(Data, Length);
			mSize += Length;
		}

		void WriteInt8(char Data, size_t Offset = 0)
		{
			Write(&Data, Offset, INT8_SIZE);
		}

		void WriteInt16(short Data, size_t Offset = 0)
		{
			Write(reinterpret_cast<char*>(&Data), Offset, INT16_SIZE);
		}

		void WriteInt32(int Data, size_t Offset = 0)
		{
			Write(reinterpret_cast<char*>(&Data), Offset, INT32_SIZE);
		}

		void WriteInt64(long long Data, size_t Offset = 0)
		{
			Write(reinterpret_cast<char*>(&Data), Offset, INT64_SIZE);
		}

		void WriteUInt8(uint8_t Data, size_t Offset = 0)
		{
			Write(reinterpret_cast<char*>(&Data), Offset, UINT8_SIZE);
		}

		void WriteUInt16(uint16_t Data, size_t Offset = 0)
		{
			Write(reinterpret_cast<char*>(&Data), Offset, UINT16_SIZE);
		}

		void WriteUInt32(uint32_t Data, size_t Offset = 0)
		{
			Write(reinterpret_cast<char*>(&Data), Offset, UINT32_SIZE);
		}

		void WriteUInt64(uint64_t Data, size_t Offset = 0)
		{
			Write(reinterpret_cast<char*>(&Data), Offset, UINT64_SIZE);
		}

		void WriteFloat(float Data, size_t Offset = 0)
		{
			Write(reinterpret_cast<char*>(&Data), Offset, FLOAT_SIZE);
		}

		void WriteDouble(double Data, size_t Offset = 0)
		{
			Write(reinterpret_cast<char*>(&Data), Offset, DOUBLE_SIZE);
		}

		void WriteString(const std::string& Data, size_t Offset = 0)
		{
			Write(Data.c_str(), Offset, strlen(Data.c_str()));
		}
	};

	class BinaryReader
	{
		InStream Stream;

	public:
		BinaryReader(const std::string& FileName)
		{
			Stream = InStream(FileName, std::ios::binary | std::ios::in);
		}
		~BinaryReader()
		{
			Close();
		}

		void   Close() { Stream.close(); }
		size_t Pos() { return Stream.tellg(); }
		void   Seek(size_t Offset, SeekMode Mode = SM_Current)
		{
			Stream.seekg(Offset, static_cast<std::ios_base::seekdir>(Mode));
		}
		int8_t Peek() { return Stream.peek(); }

		void Read(char* Data, size_t Length, size_t Offset = 0)
		{
			if (Offset > 0)
			{
				Seek(Offset);
			}
			Stream.read(Data, Length);
		}

		int8_t ReadInt8(size_t Offset = 0)
		{
			int8_t Data;
			Read(reinterpret_cast<char*>(&Data), INT8_SIZE, Offset);
			return Data;
		}

		int16_t ReadInt16(size_t Offset = 0)
		{
			int16_t Data;
			Read(reinterpret_cast<char*>(&Data), INT16_SIZE, Offset);
			return Data;
		}

		int32_t ReadInt32(size_t Offset = 0)
		{
			int32_t Data;
			Read(reinterpret_cast<char*>(&Data), INT32_SIZE, Offset);
			return Data;
		}

		int64_t ReadInt64(size_t Offset = 0)
		{
			int64_t Data;
			Read(reinterpret_cast<char*>(&Data), INT64_SIZE, Offset);
			return Data;
		}

		uint8_t ReadUInt8(size_t Offset = 0)
		{
			uint8_t Data;
			Read(reinterpret_cast<char*>(&Data), UINT8_SIZE, Offset);
			return Data;
		}

		uint16_t ReadUInt16(size_t Offset = 0)
		{
			uint16_t Data;
			Read(reinterpret_cast<char*>(&Data), UINT16_SIZE, Offset);
			return Data;
		}

		uint32_t ReadUInt32(size_t Offset = 0)
		{
			uint32_t Data;
			Read(reinterpret_cast<char*>(&Data), UINT32_SIZE, Offset);
			return Data;
		}

		uint64_t ReadUInt64(size_t Offset = 0)
		{
			uint64_t Data;
			Read(reinterpret_cast<char*>(&Data), UINT64_SIZE, Offset);
			return Data;
		}

		float ReadFloat(size_t Offset = 0)
		{
			float Data;
			Read(reinterpret_cast<char*>(&Data), FLOAT_SIZE, Offset);
			return Data;
		}

		double ReadDouble(size_t Offset = 0)
		{
			double Data;
			Read(reinterpret_cast<char*>(&Data), DOUBLE_SIZE, Offset);
			return Data;
		}

		std::string ReadString(size_t Offset = 0)
		{
			std::string Data;
			while (!Stream.eof() && Peek() != '\0')
			{
				Data.append(1, ReadInt8(Offset));
			}
			return Data;
		}
	};
} // namespace Files