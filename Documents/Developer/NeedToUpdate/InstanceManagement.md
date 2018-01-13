# インスタンスの管理責任者

インスタンス管理の責任者は、そのインスタンスをshared_ptrで保有する。
それ以外の保有者はWeakPointerで保有する。

インスタンス型:管理責任者

- SceneManager:PlanetaEngine
- Scene:SceneManager
- GameObjectManager:Scene
- GameProcessManager:Scene
- UIManager:Scene
- GameObject:GameObjectManager
- Component:GameObject
- GameProcess:GameProcessManager
- UIObject:UIManager