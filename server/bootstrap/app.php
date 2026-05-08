<?php

require_once __DIR__ . '/../vendor/autoload.php';

use App\Infrastructure\Database\DatabaseConnection;
use App\MQTT\MqttConnection;
use App\Services\SystemLogger\{SensorDataLogger, StatusLogger};

$dbConfig = require __DIR__ . '/../config/database.php';
$mqttConfig = require __DIR__ . '/../config/mqtt.php';

$sensorDataLogger = new SensorDataLogger();
$statusLogger = new StatusLogger();

$dbConnection = new DatabaseConnection($dbConfig, $statusLogger);
$pdo = $dbConnection->connect();

$mqttConnection = new MqttConnection($mqttConfig, $statusLogger);
$phpMqtt = $mqttConnection->connect();

return [
    'pdo' => $pdo,
    'phpMqtt' => $phpMqtt,
    'statusLogger' => $statusLogger,
    'sensorDataLogger' => $sensorDataLogger
];