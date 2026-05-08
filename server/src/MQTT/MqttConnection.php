<?php

namespace App\MQTT;

use App\Exceptions\Connections\MqttException;
use App\Services\SystemLogger\StatusLogger;
use Bluerhinos\phpMQTT;

class MqttConnection
{
    private array $config;
    private StatusLogger $statusLogger;

    public function __construct(array $config, StatusLogger $statusLogger)
    {
        $this->config = $config;
        $this->statusLogger = $statusLogger;
    }

    public function connect(): phpMQTT
    {
        $host = $this->config['host'];
        $port = $this->config['port'];
        $clientId = $this->config['clientId'];

        $mqtt = new phpMQTT($host, $port, $clientId);

        if ($mqtt->connect(true)) {
            $this->statusLogger->info('Successfully connected to MQTT!', [
                'host' => $host,
                'port' => $port,
                'client_id' => $clientId
            ]);

            return $mqtt;
        } else {
            $this->statusLogger->critical('Failed to connect to MQTT');

            throw new MqttException("Failed to connect to MQTT");
        }
    }
}