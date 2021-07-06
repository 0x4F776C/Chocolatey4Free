#include <iostream>
#include <fstream>
#include <fmt/core.h>

int main(int argc, char* argv[]) {
	std::ofstream file;
	std::string powershell;
	std::string packageIDName;
	std::string packageVersion;
	std::string urlPath;

	if ( argv[1] == NULL || argv[2] == NULL || argv[3] == NULL ) {
		printf("[!] Ensure all required fields have been filled\n");
		printf("\n");
		printf("[#] Switches: ./Chocolatey4Free <Package ID Name> <Package Version> <Location of CIFS Share (executable)>\n");
		printf("[#] Usage: ./Chocolatey4Free adobereader2017 17.0 '\\\\192.168.100.1\\Public\\exe\\AcroRdr20171700830051_MUI.exe'\n");
		
		return 1;
	}
	else {
		file.open("temp.ps1");

		//packageIDName = "CH";
		//packageVersion = 10.1;
		//urlPath = "\\\\192.168.100.1\\Public\\exe\\downloadMe.exe";
		packageIDName = argv[1];
		packageVersion = argv[2];
		urlPath = argv[3];

		//powershell = fmt::format("choco new {0} silentargs'/sAll /rs /msi EULA_ACCEPT=YES' url='{1}'\n", packageIDName, urlPath);
		powershell = fmt::format("New-Item -Type Directory -Name {0}\n", packageIDName);
		//powershell += fmt::format("cd {0}\n", packageIDName);
		//powershell += fmt::format("New-Item -Type File -Name {0}.nuspec\n", packageIDName);
		//powershell += fmt::format("Copy-Item default.nuspec {0}.nuspec\n", packageIDName);
		powershell += fmt::format("Invoke-WebRequest -Uri 'https://github.com/0x4F776C/Chocolatey/blob/main/files/default.nuspec' -OutFile '{0}\\{0}.nuspec'\n", packageIDName);
		powershell += fmt::format("(Get-Content {0}\\{0}.nuspec).Replace('<package id name>', '{0}') | Set-Content {0}\\{0}.nuspec\n", packageIDName);
		powershell += fmt::format("(Get-Content {0}\\{0}.nuspec).Replace('<package version>', '{1}') | Set-Content {0}\\{0}.nuspec\n", packageIDName, packageVersion);
		//powershell += fmt::format("cd tools\n");
		powershell += fmt::format("New-Item -Type Directory -Name {0}\\tools\n", packageIDName);
		powershell += fmt::format("Invoke-WebRequest -Uri 'https://github.com/0x4F776C/Chocolatey/blob/main/files/chocolateyinstall.ps1' -OutFile '{0}\\tools\\chocolateyinstall.ps1'\n", packageIDName);
		powershell += fmt::format("(Get-Content {0}\\tools\\chocolateyinstall.ps1).Replace('<package id name>', '{0}') | Set-Content {0}\\tools\\chocolateyinstall.ps1\n", packageIDName);
		powershell += fmt::format("(Get-Content {0}\\tools\\chocolateyinstall.ps1).Replace('<url>', '{1}') | Set-Content {0}\\tools\\chocolateyinstall.ps1\n", packageIDName, urlPath);
		powershell += fmt::format("Invoke-WebRequest -Uri 'https://github.com/0x4F776C/Chocolatey/blob/main/files/chocolateyuninstall.ps1' -OutFile '{0}\\tools\\chocolateyuninstall.ps1'\n", packageIDName);
		powershell += fmt::format("(Get-Content {0}\\tools\\chocolateyuninstall.ps1).Replace('<package id name>', '{0}') | Set-Content {0}\\tools\\chocolateyuninstall.ps1\n", packageIDName);
		powershell += fmt::format("cd {0}\n", packageIDName);
		powershell += "choco pack";


		file << powershell << std::endl;
		file.close();

		//system("powershell.exe -ExecutionPolicy Bypass $(Get-Content temp.ps1)");
		system("powershell.exe -ExecutionPolicy Bypass -F temp.ps1");

		remove("temp.ps1");
	}
}
