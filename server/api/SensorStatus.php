<?php

$app = require_once __DIR__ . '/../bootstrap/app.php';
use App\Controllers\API\DeviceStatusController;
use App\Repositories\Read\DeviceStatusRepository;

$repository = new DeviceStatusRepository($app['pdo']);

$controller = new DeviceStatusController($repository);
$controller->getLatestData();