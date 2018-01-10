#pragma once

//! 定義済みゲームオブジェクトコンポーネントのヘッダファイルをまとめてインクルードする
//! PlanetaEngine内部では使わないこと

//CCollider2D
#include "planeta/buildin/CCollider2D.hpp"
#include "planeta/buildin/CCircleCollider2D.hpp"
#include "planeta/buildin/CPolygonCollider2D.hpp"
#include "planeta/buildin/CStraightLineCollider2D.hpp"

//CDraw2D
#include "planeta/buildin/CDraw2D.hpp"
#include "planeta/buildin/CDrawGraph2D.hpp"
#include "planeta/buildin/CDrawLine2D.hpp"
#include "planeta/buildin/CDrawPlanet2D.hpp"
#include "planeta/buildin/CDrawPolygon2D.hpp"

//CGround2D
#include "planeta/buildin/CGround2D.hpp"
#include "planeta/buildin/CPlanetGround2D.hpp"

//Others
#include "planeta/buildin/CTransform2D.hpp"
#include "planeta/buildin/CBelongingGroundGravity.hpp"
#include "planeta/buildin/CCamera2D.hpp"
#include "planeta/buildin/CGraph2DAnimator.hpp"
#include "planeta/buildin/CPlanet.hpp"
