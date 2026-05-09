<?php

require_once __DIR__ . '/../vendor/autoload.php';

use App\Infrastructure\Database\DatabaseConnection;
use App\MQTT\{MqttConnection, MqttPublish};
use App\Services\SystemLogger\{SensorDataLogger, StatusLogger};

$dbConfig = require __DIR__ . '/../config/database.php';
$mqttConfig = require __DIR__ . '/../config/mqtt.php';

$sensorDataLogger = new SensorDataLogger();
$statusLogger = new StatusLogger();

$dbConnection = new DatabaseConnection($dbConfig, $statusLogger);
$pdo = $dbConnection->connect();

$mqttConnection = new MqttConnection($mqttConfig, $statusLogger);
$phpMqtt = $mqttConnection->connect();

$mqttPublish = new MqttPublish($phpMqtt, $statusLogger);

return [
    'pdo' => $pdo,
    'phpMqtt' => $phpMqtt,
    'mqttPublish' => $mqttPublish,
    'statusLogger' => $statusLogger,
    'sensorDataLogger' => $sensorDataLogger
];