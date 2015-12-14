#pragma once


namespace planeta_engine {
	namespace game {
		class GUIManager;
		class GUIObject;
		class GUIObjectResisterConnection final{
		public:
			GUIObjectResisterConnection(GUIManager& um, int layer, GUIObject* pointer) :ui_manager_(um), layer_(layer), pointer_(pointer) {};
			bool Show();
			bool Close();
			void Dispose();
		private:
			GUIManager& ui_manager_;
			int layer_;
			GUIObject* pointer_;
		};
	}
}