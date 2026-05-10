<?php

$app = require_once __DIR__ . '/../bootstrap/app.php';
$http = require_once __DIR__ . '/../bootstrap/http.php';

use App\Controllers\API\SensorDataController;
use App\Repositories\Read\SensorDataRepository;

$repository = new SensorDataRepository($app['pdo']);

$controller = new SensorDataController($repository);
$controller->getLatestData();