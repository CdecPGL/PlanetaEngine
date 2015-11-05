#pragma once


namespace planeta_engine {
	namespace game {
		class UIManager;
		class UIObject;
		class UIObjectResisterConnection final{
		public:
			UIObjectResisterConnection(UIManager& um, int layer, UIObject* pointer) :ui_manager_(um), layer_(layer), pointer_(pointer) {};
			bool Show();
			bool Close();
			void Dispose();
		private:
			UIManager& ui_manager_;
			int layer_;
			UIObject* pointer_;
		};
	}
}