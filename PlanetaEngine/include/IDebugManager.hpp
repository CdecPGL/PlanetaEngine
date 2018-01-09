#pragma once

#include <functional>
#include "StringUtility.hpp"
#include "Vector2D.hpp"
#include "Color.hpp"

namespace planeta {
	/*! デバッグ情報追加クラス公開インターフェイス*/
	class IDebugInformationAdder {
	public:
		virtual ~IDebugInformationAdder() = 0 {};
		/*! 文字列で行を追加する*/
		virtual void AddLine(const std::string line) = 0;
		/*! 任意の引数で行を追加する*/
		template<typename... Args>
		void AddLineV(Args&&... args) {
			AddLine(util::ConvertAndConnectToString(args...));
		}
	};
	/*! デバッグ描画クラス公開インターフェイス*/
	class IDebugDrawer {
	public:
		virtual ~IDebugDrawer() = 0 {};
		/*! 線を描画する*/
		virtual void DrawLine(const Vector2Df& spos, const Vector2Df& epos, const Color& color) = 0;
		/*! 円を描画する*/
		virtual void DrawCircle(const Vector2Df& pos, float radius, const Color& color, bool filled) = 0;
		/*! 複数の線を描画する*/
		virtual void DrawLines(const std::vector<Vector2Df>& pos_list, const Color& color) = 0;
	};
	/*! デバッグマネージャ公開インターフェイス*/
	class IDebugManager {
	public:
		virtual ~IDebugManager() = 0 {};
		/*! デバッグ情報チャンネルを作成する*/
		virtual bool CreateDebugInformationChannel(const std::string& channel_id,const std::function<void(IDebugInformationAdder&)>& handler) = 0;
		/*! デバッグ情報チャンネルを削除する*/
		virtual bool DeleteDebugInformationChannel(const std::string& channel_id) = 0;
		/*! デバッグ描画用チャンネルを作成する*/
		virtual bool CreateDebugDrawChannel(const std::string& channel_id,const std::function<void(IDebugDrawer&)> handler) = 0;
		/*! デバッグ描画用チャンネルを削除する*/
		virtual bool DeleteDebugDrawChannel(const std::string& channel_id) = 0;
	
	};
}