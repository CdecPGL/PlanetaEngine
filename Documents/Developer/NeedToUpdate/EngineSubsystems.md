# PlanetaEngineのサブシステム

サブシステム一覧と依存関係は以下のとおり。

- FileSystemManager<=LogManager
- RenderManager<=LogManager
- SaveManager<=LogManager,FileSystemManager
- ResourceManager<=LogManager,FileSystemManager
- DebugManager<=LogManager,RenderManager
- SoundManager<=LogManager
- InputManager<=LogManager
- ConfigManager<=LogManager,FileSystemManager
- SceneManager<=All
- SystemTimer
- LogManager<=SystemTimer
