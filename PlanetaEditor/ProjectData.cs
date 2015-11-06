using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PlanetaEditor
{
	public class ProjectData
	{
		private GameInfomation game_information_ = new GameInfomation();
        public GameInfomation game_information{ get { return game_information_; } set { game_information_ = value; } }
	}
}
