using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PlanetaEditor
{
	public class GameInfomation
	{
		private string title_ = "あたらしいゲーム";
		public string title { get { return title_; } set { title_ = value; } }
		private string version_ = "0.0.0";
		public string version { get { return version_; } set { version_ = value; } }
		public int internalWindth { get; set; }
		public int internalHeight { get; set; }
		public int drawWidth { get; set; }
		public int drawHeight { get; set; }
		public bool windowMode { get; set; }
		public bool drawSizeConfigurable { get; set; }
		public bool windowModeConfigurable { get; set; }
	}
}
