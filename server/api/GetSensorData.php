<?php

$app = require_once __DIR__ . '/../bootstrap/app.php';
use App\Controllers\API\SensorDataController;
use App\Repositories\Read\SensorDataRepository;
use App\Services\SystemLogger\SensorDataLogger;

$logger = new SensorDataLogger();

$repository = new SensorDataRepository($app['pdo'], $logger);

$controller = new SensorDataController($repository);
$controller->getLatestData();