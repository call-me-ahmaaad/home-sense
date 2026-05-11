<?php

$cors = require __DIR__ . '/../config/cors.php';

header("Content-Type: application/json");
header("Access-Control-Allow-Origin: " . $cors['allowed_origin']);
header("Access-Control-Allow-Methods: GET, POST, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type");

if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
    http_response_code(200);
    exit;
}