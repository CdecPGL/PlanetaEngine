using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.MemoryMappedFiles;
using System.IO;

namespace PlanetaDebugger
{
	public partial class Form1 : Form
	{
		private const string MapFileName = "PlanetaEngineDebugCommunicater";
		private const int DebugInfoMaxSize = 256;
		private const int DebugInfoCount = 10;
		private MemoryMappedFile communicater_file;

		public Form1()
		{
			InitializeComponent();
		}

		private void ConnectToPlanetaEngine()
		{
			try {
				communicater_file = MemoryMappedFile.OpenExisting(MapFileName);
			} catch (FileNotFoundException) {

			}
			
		}

		private void UpdateDebugInfoView() {
			for (int i = 0; i < DebugInfoCount; ++i) {
				debugInfoView.Items.Add("tettet");
			}
		}

		private void Form1_Load(object sender, EventArgs e) {
			ConnectToPlanetaEngine();
		}
	}
}
