<?php

require_once __DIR__ . '/vendor/autoload.php';
use App\Controllers\SensorDataController;
use App\Controllers\SensorStatusController;
use App\Infrastructure\Database\DatabaseConnection;
use App\MQTT\MqttConnection;
use App\MQTT\MqttSubscriber;
use App\Repositories\SensorDataRepository;
use App\Repositories\SensorStatusRepository;
use App\Services\SensorDataProcess;
use App\Services\SystemLogger\{SensorDataLogger, StatusLogger};

$dbConfig = require __DIR__ . '/config/databaseConfig.php';
$mqttConfig = require __DIR__ . '/config/mqttConfig.php';

$sensorDataLogger = new SensorDataLogger();
$statusLogger = new StatusLogger();

$dbConnection = new DatabaseConnection($dbConfig, $statusLogger)->connect();
$mqttConnection = new MqttConnection($mqttConfig, $statusLogger)->connect();

$sensorDataProcessor = new SensorDataProcess();

$sensorDataRepository = new SensorDataRepository($dbConnection, $sensorDataLogger);
$sensorStatusRepository = new SensorStatusRepository($dbConnection, $statusLogger);

$sensorDataController = new SensorDataController($sensorDataProcessor, $sensorDataRepository);
$sensorStatusController = new SensorStatusController($sensorStatusRepository);

$sensorDataSubscriber = new MqttSubscriber($mqttConnection, $sensorDataController, $sensorStatusController, $statusLogger);
$sensorDataSubscriber->subscribe();