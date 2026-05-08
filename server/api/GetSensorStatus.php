<?php

$app = require_once __DIR__ . '/../bootstrap/app.php';
use App\Controllers\API\DeviceStatusController;
use App\Repositories\Read\DeviceStatusRepository;
use App\Services\SystemLogger\StatusLogger;

$logger = new StatusLogger();

$repository = new DeviceStatusRepository($app['pdo'], $logger);

$controller = new DeviceStatusController($repository);
$controller->getLatestData();