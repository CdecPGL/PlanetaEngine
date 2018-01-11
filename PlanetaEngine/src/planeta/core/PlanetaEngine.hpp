#pragma once

//PlanetaEngineを使うための基本的なヘッダファイル。
//mainファイルでインクルードし、基本的にほかのファイルではインクルードしないこと

//ライブラリファイルの指定
#ifdef _DEBUG
#pragma comment(lib,"planeta/core/planetaengine_mtd.lib")
#else
#pragma comment(lib,"planetaengine_mt.lib")
#endif

#include "Game.hpp"

//Reflectionシステムに登録するためのヘッダファイル展開
//PlanetaEngineライブラリを使用するプロジェクト内で一度も、PE_REFLECTABLE_CLASSマクロ二よりトリガークラスが定義されていない、つまりヘッダファイルをインクルードがインストールされていない場合、トリガークラスが呼び出されない。
//それを避けるために、プロジェクト内で最低一回はインクルードされる、かつインクルード回数が一回に近いこのヘッダファイルに記述する。
#include "BuiltInGameObjectComponents.hpp"
#include "BuiltInTasks.hpp"
#include "BuiltInResources.hpp"

#include "planeta/core/GameObjectStandardComponent.hpp"
#include "planeta/core/IGameObject.hpp"
#include "planeta/core/IScene.hpp"
#include "IGameObjectManager.hpp"
#include "planeta/core/ITaskManager.hpp"
#include "ICollisionWorld.hpp"
#include "IDrawSystem.hpp"
#include "ITransformSystem.hpp"
#include "planeta/core/SceneSetUpper.hpp"
#include "planeta/core/SceneSetUpProxy.hpp"
#include "planeta/core/Task.hpp"
