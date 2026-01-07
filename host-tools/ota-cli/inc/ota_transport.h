#ifndef OTA_TRANSPORT_H
#define OTA_TRANSPORT_H

/**
 * @brief Send an OTA package over a specific transport (stub).
 * 
 * @param pkg_path Path to package.
 * @param port Device port (e.g. /dev/ttyUSB0).
 * @return int 0 on success.
 */
int ota_transport_send(const char* pkg_path, const char* port);

#endif /* OTA_TRANSPORT_H */
