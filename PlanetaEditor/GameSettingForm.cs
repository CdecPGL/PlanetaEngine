using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PlanetaEditor
{
	public partial class GameSettingForm : Form
	{
		private GameInfomation game_information_ = new GameInfomation();
        public GameInfomation game_information {
			get { return game_information_; }
			set {
				game_information_ = value;
				UpdateFormInformation();
			}
		}
		public GameSettingForm()
		{
            InitializeComponent();
		}

		private void UpdateFormInformation()
		{
			titleTextBox.Text = game_information.title;
			versionTextBox.Text = game_information.version;
			internalWidthBox.Value = game_information.internalWindth;
			internalHeightBox.Value = game_information.internalHeight;
			drawWidthBox.Value = game_information.drawWidth;
			drawHeightBox.Value = game_information.drawHeight;
			windowModeCheckBox.Checked = game_information.windowMode;
			//userConfigurableCheckedListBox.
		}

		private void titleTextBox_TextChanged(object sender, EventArgs e)
		{
			game_information.title = titleTextBox.Text;
		}

		private void versionTextBox_TextChanged(object sender, EventArgs e)
		{
			game_information.version = versionTextBox.Text;
		}

		private void internalWidthBox_ValueChanged(object sender, EventArgs e)
		{
			game_information.internalWindth = Decimal.ToInt32(internalWidthBox.Value);
		}

		private void internalHeightBox_ValueChanged(object sender, EventArgs e)
		{
			game_information.internalHeight = Decimal.ToInt32(internalHeightBox.Value);
		}

		private void drawWidthBox_ValueChanged(object sender, EventArgs e)
		{
			game_information.drawWidth = Decimal.ToInt32(drawWidthBox.Value);
		}

		private void drawHeightBox_ValueChanged(object sender, EventArgs e)
		{
			game_information.drawHeight = Decimal.ToInt32(drawHeightBox.Value);
		}

		private void windowModeCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			game_information.windowMode = windowModeCheckBox.Checked;
		}
	}
}
