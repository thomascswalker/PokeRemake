// ReSharper disable CppUnusedIncludeDirective

#pragma once

#include "CFwd.h"

/* Types */

struct PColor;
template <typename T>
struct TVector2;
template <typename T>
struct TVector3;
template <typename T>
struct TVector4;
template <typename T>
struct TMatrix;
template <typename T>
struct TRect;

using FVector2 = TVector2<float>;
using FVector3 = TVector3<float>;
using FVector4 = TVector4<float>;
using IVector2 = TVector2<int32_t>;
using IVector3 = TVector3<int32_t>;
using IVector4 = TVector4<int32_t>;
using FMatrix = TMatrix<float>;
using DMatrix = TMatrix<double>;
using IMatrix = TMatrix<int32_t>;
using FRect = TRect<float>;
using IRect = TRect<int32_t>;

/* Classes */

class PGameInstance;
class PAbstractButton;
class PActor;
class PBox;
class PButton;
class PButtonGroup;
class PCameraComponent;
class PCameraView;
class PCharacter;
class PCharacterMovementComponent;
class PCollisionComponent;
class PComponent;
class PDropdown;
class PDropdownView;
class PEngine;
class PGame;
class PGridView;
class PGroup;
class PHUD;
class PImage;
class PInteractionComponent;
class PGameMap;
class PObject;
class PPanel;
class PPlayerCharacter;
class PPortal;
class PRenderer;
class PScrollArea;
class PSelectionComponent;
class PSpinner;
class PSprite;
class PText;
class PWidget;
class PWorld;
class PGameMode;

// Structs
struct SGameState;

/* Interfaces */

class ICollider;
class IInputHandler;
class IInputManager;
class IInteractable;
class ISelectable;
