#pragma once

//PlanetaEngineを使うための基本的なヘッダファイル。
//mainファイルでインクルードし、基本的にほかのファイルではインクルードしないこと

////ライブラリファイルの指定
//#ifdef _DEBUG
//#pragma comment(lib,"planetaengine_mtd.lib")
//#else
//#pragma comment(lib,"planetaengine_mt.lib")
//#endif

#include "core/Game.hpp"

//Reflectionシステムに登録するためのヘッダファイル展開
//PlanetaEngineライブラリを使用するプロジェクト内で一度も、PE_REFLECTABLE_CLASSマクロ二よりトリガークラスが定義されていない、つまりヘッダファイルをインクルードがインストールされていない場合、トリガークラスが呼び出されない。
//それを避けるために、プロジェクト内で最低一回はインクルードされる、かつインクルード回数が一回に近いこのヘッダファイルに記述する。
#include "BuildinGameObjectComponents.hpp"
#include "BuildinTasks.hpp"
#include "BuildinResources.hpp"

#include "core/GameObjectStandardComponent.hpp"
#include "core/IGameObject.hpp"
#include "core/IScene.hpp"
#include "core/IGameObjectManager.hpp"
#include "core/ITaskManager.hpp"
#include "core/ICollisionWorld.hpp"
#include "core/IDrawSystem.hpp"
#include "core/ITransformSystem.hpp"
#include "core/IInputManager.hpp"
#include "core/SceneSetUpper.hpp"
#include "core/SceneSetUpProxy.hpp"
#include "core/Task.hpp"
#include "core/EACollisionWithCollider2D.hpp"
#include "core/EACollisionWithGround2D.hpp"
