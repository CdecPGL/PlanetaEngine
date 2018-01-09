#pragma once

//! 定義済みゲームオブジェクトコンポーネントのヘッダファイルをまとめてインクルードする
//! PlanetaEngine内部では使わないこと

//CCollider2D
#include "CCollider2D.hpp"
#include "CCircleCollider2D.hpp"
#include "CPolygonCollider2D.hpp"
#include "CStraightLineCollider2D.hpp"

//CDraw2D
#include "CDraw2D.hpp"
#include "CDrawGraph2D.hpp"
#include "CDrawLine2D.hpp"
#include "CDrawPlanet2D.hpp"
#include "CDrawPolygon2D.hpp"

//CGround2D
#include "CGround2D.hpp"
#include "CPlanetGround2D.hpp"

//Others
#include "CTransform2D.hpp"
#include "CBelongingGroundGravity.hpp"
#include "CCamera2D.hpp"
#include "CGraph2DAnimator.hpp"
#include "CPlanet.hpp"
