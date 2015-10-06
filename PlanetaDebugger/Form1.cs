using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Pipes;
using System.Runtime.InteropServices;

namespace PlanetaDebugger
{

	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
			
		}

		[DllImport("kernel32.dll")]
		private static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, IntPtr lpBuffer, uint nSize, UIntPtr lpNumberOfBytesRead);
		[DllImport("kernel32.dll")]
		private static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, IntPtr lpBuffer, uint nSize, UIntPtr lpNumberOfBytesWritten);

		private void ConnectToPlanetaEngine()
		{
			//名前付きパイプを作成
			NamedPipeClientStream pipe = new NamedPipeClientStream(".", "PlanetaEngineDebugPipe",PipeDirection.InOut,PipeOptions.None);
			//接続待機
			while (!pipe.IsConnected)
			{
				pipe.Connect(3000);
			}
			//デバッグ対象のプロセスIDを受け取る
			byte[] buffer = new byte[256];
			pipe.Read(buffer, 0, 4);
			uint pid = (uint)buffer[0] + ((uint)buffer[1] << 8) + ((uint)buffer[2] << 16) + ((uint)buffer[3] << 24);
			//プロセスをIDから取得
			System.Diagnostics.Process hProcess = System.Diagnostics.Process.GetProcessById((int)pid);
			IntPtr phandle = hProcess.Handle;
			//送受信テスト

			
		}
	}
}
