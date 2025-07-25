// ReSharper disable CppUnusedIncludeDirective

#pragma once

#include "CFwd.h"

/* Types */

struct PColor;
template <typename T>
class TVector2;
template <typename T>
class TVector3;
template <typename T>
class TVector4;
template <typename T>
class TMatrix;
template <typename T>
class TRect;

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

class PAbstractButton;
class PActor;
class PBox;
class PButton;
class PButtonGroup;
class PCameraComponent;
class PCameraView;
class PCanvas;
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
class PImage;
class PMap;
class PObject;
class PPanel;
class PPlayerCharacter;
class PPortal;
class PRenderer;
class PScrollArea;
class PSpinner;
class PSprite;
class PText;
class PWidget;
class PWorld;

/* Interfaces */

class ICollider;
class IInputHandler;
class IInputManager;
class IInteractable;
class ISelectable;
