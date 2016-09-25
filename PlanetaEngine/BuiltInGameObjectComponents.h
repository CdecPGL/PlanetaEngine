#pragma once

//! 定義済みゲームオブジェクトコンポーネントのヘッダファイルをまとめてインクルードする
//! PlanetaEngine内部では使わないこと

//CCollider2D
#include "CCollider2D.h"
#include "CCircleCollider2D.h"
#include "CPolygonCollider2D.h"
#include "CStraightLineCollider2D.h"

//CDraw2D
#include "CDraw2D.h"
#include "CDrawGraph2D.h"
#include "CDrawLine2D.h"
#include "CDrawPlanet2D.h"
#include "CDrawPolygon2D.h"

//CGround2D
#include "CGround2D.h"
#include "CPlanetGround2D.h"

//Others
#include "CTransform2D.h"
#include "CBelongingGroundGravity.h"
#include "CCamera2D.h"
#include "CGraph2DAnimator.h"
#include "CPlanet.h"
