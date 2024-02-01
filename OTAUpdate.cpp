// OTAUpdate.cpp

#include "OTAUpdate.h"
#include <TelnetStream.h>
#include <WiFi.h>
const char* rootCACertificate = \
  // Your root CA certificate content
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\n" \
  "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
  "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n" \
  "MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
  "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
  "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\n" \
  "9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\n" \
  "2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\n" \
  "1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\n" \
  "q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\n" \
  "tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\n" \
  "vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\n" \
  "BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\n" \
  "5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\n" \
  "1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\n" \
  "NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\n" \
  "Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\n" \
  "8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\n" \
  "pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\n" \
  "MrY=\n" \
  "-----END CERTIFICATE-----";

void OTAUpdater::update_started() {
  Serial.println("HTTPS update process started");
  TelnetStream.println("HTTPS update process started");
}

void OTAUpdater::update_finished() {
  Serial.println("HTTPS update process finished");
  TelnetStream.println("HTTPS update process finished");
}

void OTAUpdater::update_progress(int cur, int total) {
  // Update progress
  static int latestProgress = -1;
  int progress = (cur*100)/total;
  if(progress>latestProgress){
    latestProgress =progress;
  Serial.printf("Downloading firmware: %d%%\n", progress);
  TelnetStream.print("Downloading firmare: ");
  TelnetStream.print(progress);
  TelnetStream.println("%");
  //Serial.printf("Downloading %d of %d bytes...\n", cur, total);
  }
}

void OTAUpdater::update_error(int err) {
  Serial.printf("CALLBACK: HTTPS update fatal error code %d\n", err);
  TelnetStream.print("CALLBACK: HTTPS update fatal error code: ");
  TelnetStream.println(err);
}

void OTAUpdater::checkForUpdate(const String &projectName, const String &currentVersion) {
  // Define the update URL in this file
    String deviceName ="ESP-"+ String(ESP.getEfuseMac(),HEX);
    String macAddr = WiFi.macAddress();
    String fileName = projectName+".ino.bin"; //firmware.bin on vscode
    

    WiFiClientSecure client;  // Use WiFiClientSecure for HTTPS

    // Define the update URL in this file
    
     String updateUrl = "https://otaup.000webhostapp.com/update_server.php?project_name=" + projectName + "&file_name=" + currentVersion + "_" + fileName+"&device_name="+deviceName+"&device_macaddress="+macAddr;
     Serial.println("\nDevice: "+deviceName+" | Project Name: " + projectName + " | Version: v" + currentVersion);
     TelnetStream.println("\nDevice: "+deviceName+" | Project Name: " + projectName + " | Version: v" + currentVersion);
    Serial.println("Checking for firmware updates...");
    TelnetStream.println("\nChecking for firmware updates...");
    /*if(debugMode){
      Serial.println(updateUrl);
    }*/
    // Set the root CA certificate for secure communication
    client.setCACert(rootCACertificate);

    HTTPClient http;

    if (http.begin(client, updateUrl)) {
      int httpCode = http.GET();
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.print(payload);
        TelnetStream.print(payload);
        if (payload.equals("No update available.")) {
          Serial.println("\nDevice: "+deviceName+" | Project Name: " + projectName + " | Version: v" + currentVersion);
          TelnetStream.println("\nDevice: "+deviceName+" | Project Name: " + projectName + " | Version: v" + currentVersion);
        } else {
          Serial.println("Update available. \nDownloading and installing...");
          TelnetStream.println("Update available. \nDownloading and installing...");
          httpUpdate.onStart(update_started);
          httpUpdate.onEnd(update_finished);
          httpUpdate.onProgress(update_progress);
          httpUpdate.onError(update_error);

          t_httpUpdate_return ret = httpUpdate.update(client, updateUrl.c_str());

          switch (ret) {
            case HTTP_UPDATE_FAILED:
              Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
              break;

            case HTTP_UPDATE_NO_UPDATES:
              Serial.println("HTTP_UPDATE_NO_UPDATES");
              break;

            case HTTP_UPDATE_OK:
              Serial.println("HTTP_UPDATE_OK");
              // Perform any additional tasks after a successful update if needed
              break;
          }
        }
      } else {
        Serial.printf("HTTP request failed with error code: %d\n", httpCode);
        TelnetStream.print("HTTP request failed with error code: ");
        TelnetStream.println(httpCode);
      }
      http.end();
    } else {
      Serial.println("Unable to connect to the server.");
      TelnetStream.print("Unable to connect to the server.");
      
    }
  
}
