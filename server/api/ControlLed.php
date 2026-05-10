<?php

$app = require_once __DIR__ . '/../bootstrap/app.php';
$http = require_once __DIR__ . '/../bootstrap/http.php';

use App\Controllers\API\ControlLedController;

$controller = new ControlLedController($app['mqttPublish'], $app['statusLogger']);
$controller->receiveData();