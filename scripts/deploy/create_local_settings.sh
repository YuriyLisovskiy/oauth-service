#!/bin/sh

SECRET_KEY=$1

DB_NAME=$2
DB_USER=$3
DB_PASSWORD=$4
DB_HOST=$5
DB_PORT=$6
DB_CONNECTIONS=$7

OAUTH_JWT_SUBJECT=$8
OAUTH_JWT_ISSUER=$9
OAUTH_JWT_PERIOD_MIN=${10}
OAUTH_JWT_PERIOD_DAYS=${11}
OAUTH_SIGNER_ALGORITHM=${12}
OAUTH_SIGNER_SECRET_KEY=${13}

echo "secret_key: '${SECRET_KEY}'

allowed_hosts:
  - '*'

logger:
  levels:
    debug: false

databases:
  - instance: default
    dbms: postgresql
    name: ${DB_NAME}
    user: ${DB_USER}
    password: ${DB_PASSWORD}
    host: ${DB_HOST}
    port: ${DB_PORT}
    connections: ${DB_CONNECTIONS}

oauth:
  jwt:
    subject: '${OAUTH_JWT_SUBJECT}'
    issuer: '${OAUTH_JWT_ISSUER}'
    period:
      minutes: ${OAUTH_JWT_PERIOD_MIN}
      days: ${OAUTH_JWT_PERIOD_DAYS}
  signer:
    algorithm: ${OAUTH_SIGNER_ALGORITHM}
    secret_key: '${OAUTH_SIGNER_SECRET_KEY}'
"
