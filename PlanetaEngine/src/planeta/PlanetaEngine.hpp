#pragma once

//PlanetaEngineを使うための基本的なヘッダファイル。
//mainファイルでインクルードし、基本的にほかのファイルではインクルードしないこと

////ライブラリファイルの指定
//#ifdef _DEBUG
//#pragma comment(lib,"planetaengine_mtd.lib")
//#else
//#pragma comment(lib,"planetaengine_mt.lib")
//#endif

#include "core/game.hpp"

//Reflectionシステムに登録するためのヘッダファイル展開
//PlanetaEngineライブラリを使用するプロジェクト内で一度も、PE_REFLECTABLE_CLASSマクロ二よりトリガークラスが定義されていない、つまりヘッダファイルをインクルードがインストールされていない場合、トリガークラスが呼び出されない。
//それを避けるために、プロジェクト内で最低一回はインクルードされる、かつインクルード回数が一回に近いこのヘッダファイルに記述する。
#include "BuildinGameObjectComponents.hpp"
#include "BuildinTasks.hpp"
#include "BuildinResources.hpp"

#include "core\game_object_standard_component.hpp"
#include "core\i_game_object.hpp"
#include "core/IScene.hpp"
#include "core\i_game_object_manager.hpp"
#include "core/ITaskManager.hpp"
#include "core/i_collision_world.hpp"
#include "core\i_draw_system.hpp"
#include "core/ITransformSystem.hpp"
#include "core\i_input_manager.hpp"
#include "core/SceneSetUpper.hpp"
#include "core/SceneSetUpProxy.hpp"
#include "core/Task.hpp"
#include "core/e_collision_with_collider_2d.hpp"
#include "core/e_collision_with_ground_2d.hpp"
