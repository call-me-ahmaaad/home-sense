<?php

namespace App\Infrastructure\Database;

use PDO;
use PDOException;
use App\Exceptions\Connections\DatabaseException;
use App\Services\SystemLogger\StatusLogger;

class DatabaseConnection
{
    private array $config;
    private StatusLogger $statusLogger;

    public function __construct(array $config, StatusLogger $statusLogger)
    {
        $this->config = $config;
        $this->statusLogger = $statusLogger;
    }

    public function connect(): PDO
    {
        $host = $this->config['host'];
        $username = $this->config['username'];
        $password = $this->config['password'];
        $dbName = $this->config['dbName'];

        try {
            $conn = new PDO("mysql:host=$host;dbname=$dbName", $username, $password);

            $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            echo "[INFO][SUCCESS] Successfully connected to database!" . PHP_EOL;
            $this->statusLogger->info('Successfully connected to database', [
                'host' => $host,
                'db_name' => $dbName
            ]);

            return $conn;
        } catch (PDOException $error) {
            $this->statusLogger->critical('Failed to connect to database');

            throw new DatabaseException("[ERROR][CONNECTION ERROR] Failed to connect to database!");
        }
    }
}