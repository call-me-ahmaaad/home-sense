<?php

$app = require_once __DIR__ . '/bootstrap/app.php';

use App\Services\SensorDataProcess;
use App\Repositories\Write\SensorDataRepository;
use App\Repositories\Write\DeviceStatusRepository;
use App\Controllers\MQTT\SensorDataController;
use App\Controllers\MQTT\DeviceStatusController;
use App\MQTT\MqttSubscriber;

$sensorDataProcessor = new SensorDataProcess();

$sensorDataRepository = new SensorDataRepository(
    $app['pdo'],
    $app['sensorDataLogger']
);

$deviceStatusRepository = new DeviceStatusRepository(
    $app['pdo'],
    $app['statusLogger']
);

$sensorDataController = new SensorDataController(
    $sensorDataProcessor,
    $sensorDataRepository
);

$deviceStatusController = new DeviceStatusController($deviceStatusRepository);

$sensorDataSubscriber = new MqttSubscriber(
    $app['phpMqtt'],
    $sensorDataController,
    $deviceStatusController,
    $app['statusLogger']
);

$sensorDataSubscriber->subscribe();