#pragma once

//! 定義済みゲームオブジェクトコンポーネントのヘッダファイルをまとめてインクルードする
//! PlanetaEngine内部では使わないこと

//CCollider2D
#include "buildin/c_collider_2d.hpp"
#include "buildin/c_circle_collider_2d.hpp"
#include "buildin/c_polygon_collider_2d.hpp"
#include "buildin/c_straight_line_collider_2d.hpp"

//CDraw2D
#include "buildin/c_draw_2d.hpp"
#include "buildin/c_draw_graph_2d.hpp"
#include "buildin/c_draw_line_2d.hpp"
#include "buildin/c_draw_planet_2d.hpp"
#include "buildin/c_draw_polygon_2d.hpp"

//CGround2D
#include "buildin/c_ground_2d.hpp"
#include "buildin/c_planet_ground_2d.hpp"

//Others
#include "buildin/c_transform_2d.hpp"
#include "buildin/c_belonging_ground_gravity.hpp"
#include "buildin/c_camera_2d.hpp"
#include "buildin/c_graph_2d_animator.hpp"
#include "buildin/c_planet.hpp"
#include "buildin/c_effect.hpp"
