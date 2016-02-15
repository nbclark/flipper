using System;
using System.Collections.Generic;
using System.IO;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace MobileSRC.GScroll
{
    class ApplicationListView : ListView
    {
        static string[][] _appActions = new string[][]
        {
            new string[] { "Launch Start Menu", "::start" } ,
            new string[] { "Close Current Application", "::close" },
            new string[] { "Click OK Button", "::ok" },
            new string[] { "Minimize Current Application", "::minimize" },
            new string[] { "Left Soft Key", "::leftsoft" },
            new string[] { "Right Soft Key", "::rightsoft" },
            new string[] { "Lock Phone", "::lock" },
            new string[] { "Tap & Tilt", "::taptilt" },
            new string[] { "Launch AppSwitch", "::appswitch" },
            new string[] { "Launch AppSwitch Forward", "::appswitchnext" },
            new string[] { "Launch AppSwitch Previous", "::appswitchprev" },
            new string[] { "Type Text", "::type enter text here" }
        };

        [DllImport("coredll.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool DestroyIcon(IntPtr hIcon);
        [DllImport("gscrollsetup.dll", SetLastError = true)]
        static extern IntPtr LoadApplicationIcon(string szFilteredPath, bool bSmall, string wzExeFile, string wzWndName, string wzClassName);
        [DllImport("gscrollsetup.dll", SetLastError = true)]
        public static extern bool GetProcessName(string szLinkName, StringBuilder lpString, int nMaxCount);

        private static bool _isInitialized = false;
        private static ImageList _imageList = new ImageList();
        private static List<ListViewItem> _listItems = new List<ListViewItem>();
        private System.Windows.Forms.ColumnHeader _iconColumnHeader;
        private System.Windows.Forms.ColumnHeader _actionColumnHeader;

        public ApplicationListView()
        {
            this._iconColumnHeader = new System.Windows.Forms.ColumnHeader();
            this._actionColumnHeader = new System.Windows.Forms.ColumnHeader();
            // 
            // _iconColumnHeader
            // 
            this._iconColumnHeader.Text = "";
            this._iconColumnHeader.Width = 40;
            // 
            // _actionColumnHeader
            // 
            this._actionColumnHeader.Text = "Launch Action";
            this._actionColumnHeader.Width = 412;

            this.Columns.Add(this._iconColumnHeader);
            this.Columns.Add(this._actionColumnHeader);
            this.FullRowSelect = true;
            this.View = System.Windows.Forms.View.Details;

            Initialize();
        }

        private void RecurseGetLnkFiles(string folder, List<string> files)
        {
            foreach (string file in Directory.GetFiles(folder, "*.lnk"))
            {
                files.Add(file);
            }
            foreach (string dir in Directory.GetDirectories(folder))
            {
                RecurseGetLnkFiles(dir, files);
            }
        }

        private void Initialize()
        {
            if (!_isInitialized)
            {
                Cursor.Current = Cursors.WaitCursor;

                _imageList.ImageSize = new Size(32, 32);
                _imageList.Images.Add(Properties.Resources.app);

                List<string> files = new List<string>();
                RecurseGetLnkFiles(Environment.GetFolderPath(Environment.SpecialFolder.StartMenu), files);

                files.Sort();

                foreach (string[] appSetting in _appActions)
                {
                    ListViewItem listItem = new ListViewItem(new string[] { string.Empty, appSetting[0] });
                    listItem.ImageIndex = 0;
                    listItem.Tag = appSetting[1];

                    _listItems.Add(listItem);
                }

                foreach (string file in files)
                {
                    if (Path.GetFileName(file) != "icon.lnk")
                    {
                        int imageIndex = 0;
                        IntPtr test = LoadApplicationIcon(file, true, null, null, null);
                        if (IntPtr.Zero != test)
                        {
                            Icon x = Icon.FromHandle(test);
                            _imageList.Images.Add(x);
                            imageIndex = _imageList.Images.Count - 1;
                            DestroyIcon(x.Handle);
                        }
                        ListViewItem listItem = new ListViewItem(new string[] { string.Empty, Path.GetFileNameWithoutExtension(file) });
                        listItem.ImageIndex = imageIndex;
                        listItem.Tag = file;

                        _listItems.Add(listItem);
                    }
                }
                _isInitialized = true;

                Cursor.Current = Cursors.Default;
            }

            this.SmallImageList = _imageList;
            foreach (ListViewItem item in _listItems)
            {
                this.Items.Add(item);
            }
        }

        protected override void Dispose(bool disposing)
        {
            this.Items.Clear();
            base.Dispose(disposing);
        }
    }
}
