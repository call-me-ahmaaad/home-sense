<?php

namespace App\Services\SystemLogger;

use Monolog\Logger;
use Monolog\Handler\StreamHandler;

class SensorDataLogger
{
    private Logger $logger;

    public function __construct()
    {
        $this->logger = new Logger('homesense');

        $this->logger->pushHandler(
            new StreamHandler(__DIR__ . '/../../../logs/data.log', Logger::DEBUG)
        );
    }

    public function info(string $message, array $context = []): void
    {
        $this->logger->info($message, $context);
    }

    public function error(string $message, array $context = []): void
    {
        $this->logger->error($message, $context);
    }
}