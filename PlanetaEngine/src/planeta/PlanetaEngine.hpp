#pragma once

//PlanetaEngineを使うための基本的なヘッダファイル。
//mainファイルでインクルードし、基本的にほかのファイルではインクルードしないこと

//ライブラリファイルの指定
#ifdef _DEBUG
#pragma comment(lib,"planeta/planetaengine_mtd.lib")
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

#include "planeta/GameObjectStandardComponent.hpp"
#include "planeta/IGameObject.hpp"
#include "planeta/IScene.hpp"
#include "IGameObjectManager.hpp"
#include "planeta/ITaskManager.hpp"
#include "ICollisionWorld.hpp"
#include "IDrawSystem.hpp"
#include "ITransformSystem.hpp"
#include "planeta/SceneSetUpper.hpp"
#include "planeta/SceneSetUpProxy.hpp"
#include "planeta/Task.hpp"
