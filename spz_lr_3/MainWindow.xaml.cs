using System;
using System.Collections.Generic;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Linq;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace spz_lr_3
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        #region Constructors
        public MainWindow()
        {
            InitializeComponent();
        }
        #endregion

        #region Button Actions
        private void B_CreateSharedMemory_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (TB_Message.Text != String.Empty)
                {
                    char[] message = TB_Message.Text.ToCharArray();
                    int size = message.Length;

                    MemoryMappedFile sharedMemory = MemoryMappedFile.CreateNew("MemoryFile", size * 2 + 4, MemoryMappedFileAccess.ReadWriteExecute);
                    using (MemoryMappedViewAccessor writer = sharedMemory.CreateViewAccessor(0, size * 2 + 4))
                    {
                        writer.Write(0, size);
                        writer.WriteArray<char>(4, message, 0, message.Length);
                    }
                }
                else
                {
                    MessageBox.Show("Text box should be filled with some data!", "Warning", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                }
            }
            catch
            {
                MessageBox.Show("Couldn't write to Shared Memory inserted data!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
        private void B_ViewSharedMemory_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                char[] message;
                int size;

                MemoryMappedFile sharedMemory = MemoryMappedFile.OpenExisting("MemoryFile");
                using (MemoryMappedViewAccessor reader = sharedMemory.CreateViewAccessor(0, 4, MemoryMappedFileAccess.Read))
                {
                    size = reader.ReadInt32(0);
                }

                using (MemoryMappedViewAccessor reader = sharedMemory.CreateViewAccessor(4, size * 2, MemoryMappedFileAccess.Read))
                {
                    message = new char[size];
                    reader.ReadArray<char>(0, message, 0, size);
                }

                TB_SharedMemory.Text = "Shared Memory data: ";
                foreach (char ch in message)
                {
                    TB_SharedMemory.Text += ch;
                }
            }
            catch
            {
                TB_SharedMemory.Text = "Shared Memory data: error!\nCouldn't read data from Shared Memory!";
            }
        }
        private void B_GetAccessRules_Click(object sender, RoutedEventArgs e)
        {
            TB_AccessRules.Text = "Access rules:\n";

            if (CB_UseSpecificFilePath.IsChecked.Value)
            {
                try
                {
                    using (FileStream myFile = new FileStream(
                        TB_SpecificFilePath.Text, FileMode.Open, FileAccess.Read))
                    {
                        FileSecurity fileSec = myFile.GetAccessControl();
                        foreach (FileSystemAccessRule fileRule in
                            fileSec.GetAccessRules(true, true, typeof(NTAccount)))
                        {
                            string accessControlType = fileRule.AccessControlType == AccessControlType.Allow ? "granted" : "denied";
                            TB_AccessRules.Text += TB_SpecificFilePath.Text + " access " + accessControlType + " "
                                + fileRule.FileSystemRights + " for " + fileRule.IdentityReference + "\n";
                            accessControlType = String.Empty;
                        }
                    }
                }
                catch
                {
                    TB_AccessRules.Text = "Couldn't find/open the file!";
                }
            }
            else
            {
                try
                {
                    MemoryMappedFile sharedMemory = MemoryMappedFile.OpenExisting("MemoryFile", MemoryMappedFileRights.ReadPermissions);

                    MemoryMappedFileSecurity mmfSec = sharedMemory.GetAccessControl();
                    foreach (AccessRule<MemoryMappedFileRights> fileRule in
                        mmfSec.GetAccessRules(true, true, typeof(NTAccount)))
                    {
                        string accessControlType = fileRule.AccessControlType == AccessControlType.Allow ? "granted" : "denied";
                        TB_AccessRules.Text += "Shared Memory " + accessControlType + " access "
                            + fileRule.Rights + " for " + fileRule.IdentityReference + "\n";
                        accessControlType = String.Empty;
                    }
                }
                catch
                {
                    TB_AccessRules.Text = "Couldn't find/open the Shared Memory!";
                }
            }
        }
        private void B_SetAccessRules_Click(object sender, RoutedEventArgs e)
        {
            TB_AccessRolesChangeResult.Text = String.Empty;

            if ((TB_SpecificFilePath.Text != String.Empty) && TB_SpecificFilePath.IsEnabled)
            {
                try
                {
                    using (FileStream myFile = new FileStream(
                        TB_SpecificFilePath.Text, FileMode.Open, FileAccess.Read))
                    {
                        FileSecurity fileSec = myFile.GetAccessControl();

                        TB_AccessRolesChangeResult.Text += "ACL list before changes:\n";
                        foreach (FileSystemAccessRule fileRule in
                            fileSec.GetAccessRules(true, true, typeof(NTAccount)))
                        {
                            string accessControlType = fileRule.AccessControlType == AccessControlType.Allow ? "granted" : "denied";
                            TB_AccessRolesChangeResult.Text += TB_SpecificFilePath.Text + " access " + accessControlType + " "
                                + fileRule.FileSystemRights + " for " + fileRule.IdentityReference + "\n";
                            accessControlType = String.Empty;
                        }

                        using (WindowsIdentity currentIdentity = WindowsIdentity.GetCurrent())
                        {
                            FileSystemAccessRule newRule = new FileSystemAccessRule(
                                new NTAccount(currentIdentity.Name),
                                (RB_Read.IsChecked.Value) ? FileSystemRights.Read : (RB_Write.IsChecked.Value) ? FileSystemRights.Write : FileSystemRights.FullControl,
                                (RB_Allow.IsChecked.Value) ? AccessControlType.Allow : AccessControlType.Deny
                            );
                            fileSec.AddAccessRule(newRule);
                        }
                        File.SetAccessControl(TB_SpecificFilePath.Text, fileSec);

                        TB_AccessRolesChangeResult.Text += "\nACL list after changes:\n";
                        foreach (FileSystemAccessRule fileRule in
                                fileSec.GetAccessRules(true, true, typeof(NTAccount)))
                        {
                            string accessControlType = fileRule.AccessControlType == AccessControlType.Allow ? "granted" : "denied";
                            TB_AccessRolesChangeResult.Text += TB_SpecificFilePath.Text + " access " + accessControlType + " "
                                + fileRule.FileSystemRights + " for " + fileRule.IdentityReference + "\n";
                            accessControlType = String.Empty;
                        }
                    }
                }
                catch
                {
                    TB_AccessRolesChangeResult.Text = "Incorrect file path, access rule or NT Account!";
                }
            }
            else
            {
                try
                {
                    MemoryMappedFile sharedMemory = MemoryMappedFile.OpenExisting("MemoryFile", MemoryMappedFileRights.ChangePermissions | MemoryMappedFileRights.ReadPermissions);

                    MemoryMappedFileSecurity mmfSec = sharedMemory.GetAccessControl();

                    TB_AccessRolesChangeResult.Text += "ACL list before changes:\n";
                    foreach (AccessRule<MemoryMappedFileRights> fileRule in
                        mmfSec.GetAccessRules(true, true, typeof(NTAccount)))
                    {
                        string accessControlType = fileRule.AccessControlType == AccessControlType.Allow ? "granted" : "denied";
                        TB_AccessRolesChangeResult.Text += "Shared Memory " + accessControlType + " access "
                            + fileRule.Rights + " for " + fileRule.IdentityReference + "\n";
                        accessControlType = String.Empty;
                    }

                    using (WindowsIdentity currentIdentity = WindowsIdentity.GetCurrent())
                    {
                        AccessRule<MemoryMappedFileRights> newRule = new AccessRule<MemoryMappedFileRights>(
                            new NTAccount(currentIdentity.Name),
                            (RB_Read.IsChecked.Value) ? MemoryMappedFileRights.Read : (RB_Write.IsChecked.Value) ? MemoryMappedFileRights.Write : MemoryMappedFileRights.FullControl,
                            (RB_Allow.IsChecked.Value) ? AccessControlType.Allow : AccessControlType.Deny
                        );
                        mmfSec.AddAccessRule(newRule);
                    }
                    sharedMemory.SetAccessControl(mmfSec);

                    TB_AccessRolesChangeResult.Text += "\nACL list after changes:\n";
                    foreach (AccessRule<MemoryMappedFileRights> fileRule in
                        mmfSec.GetAccessRules(true, true, typeof(NTAccount)))
                    {
                        string accessControlType = fileRule.AccessControlType == AccessControlType.Allow ? "granted" : "denied";
                        TB_AccessRolesChangeResult.Text += "Shared Memory " + accessControlType + " access "
                            + fileRule.Rights + " for " + fileRule.IdentityReference + "\n";
                        accessControlType = String.Empty;
                    }
                }
                catch
                {
                    TB_AccessRolesChangeResult.Text = "Incorrect name of Shared Memory, access rule or NT Account!";
                }
            }
        }
        #endregion

        #region CheckBox Actions
        private void CB_UseSpecificFilePath_Checked(object sender, RoutedEventArgs e)
        {
            TB_SpecificFilePath.IsEnabled = true;
        }
        private void CB_UseSpecificFilePath_Unchecked(object sender, RoutedEventArgs e)
        {
            TB_SpecificFilePath.IsEnabled = false;
        }
        #endregion

    }
}
