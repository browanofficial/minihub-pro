/* The config header is always included first. */
#include "iot_config.h"

#include "types/iot_mqtt_types.h"

#define WLR_MODEL_NAME		"TB-300"

/*-----------------------------------------------------------*/

/* Declaration of demo function. */
int RunHelloWorldDemo(bool awsIotMqttMode,
					  const char *pIdentifier,
					  void *pNetworkServerInfo, 
					  void *pNetworkCredentialInfo, 
					  const IotNetworkInterface_t * pNetworkInterface);

/*-----------------------------------------------------------*/

/**
 * @brief The function that runs the MQTT demo, called by the demo runner.
 *
 * @param[in] awsIotMqttMode Specify if this demo is running with the AWS IoT
 * MQTT server. Set this to `false` if using another MQTT server.
 * @param[in] pIdentifier NULL-terminated MQTT client identifier.
 * @param[in] pNetworkServerInfo Passed to the MQTT connect function when
 * establishing the MQTT connection.
 * @param[in] pNetworkCredentialInfo Passed to the MQTT connect function when
 * establishing the MQTT connection.
 * @param[in] pNetworkInterface The network interface to use for this demo.
 *
 * @return `EXIT_SUCCESS` if the demo completes successfully; `EXIT_FAILURE` otherwise.
 */
int RunHelloWorldDemo(bool awsIotMqttMode,
					  const char *pIdentifier,
					  void *pNetworkServerInfo, 
					  void *pNetworkCredentialInfo, 
					  const IotNetworkInterface_t * pNetworkInterface)
{
	/* Return value of this function and the exit status of this program. */
	int status = EXIT_SUCCESS;

	configPRINTF(("***************************\n"));
	configPRINTF(("%s >> Hello World !!!!!\n", WLR_MODEL_NAME));
	configPRINTF(("***************************\n"));

	return status;
}
