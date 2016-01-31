#pragma once

#include <memory>
#include "PointerSingletonTemplate.h"

namespace planeta_engine {
	namespace utility {
		class DataContainer;
	}
	namespace file_system {
		class FileAccessor;
	}
	namespace core {
		class SaveDataManager final: public utility::PointerSingletonTemplate<SaveDataManager>{
			friend class utility::PointerSingletonTemplate<SaveDataManager>;
		public:
			void SetFileAccessor(const std::shared_ptr<file_system::FileAccessor>& file_accessor);
			bool Initialize()override;
			bool Finalize()override;
			/*保存*/
			void Save();
			/*ユーザーデータを読み込み*/
			void LoadUserData(int idx);
			const utility::DataContainer& common_data()const;
			utility::DataContainer& common_data();
			const utility::DataContainer& current_user_data()const;
			utility::DataContainer& current_user_data();

		private:
			SaveDataManager();
			~SaveDataManager();
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	}
}