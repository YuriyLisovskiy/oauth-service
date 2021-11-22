## oauth service
OAuth service.

### API
#### OAuth
* Retrieve JWT token using `client_credentials` flow.
  Anyone with client id and client secret key can use this endpoint.
  <br/><br/>
  `[GET] /oauth/token`
  <br/><br/>
  Parameters

  | Name | Type | In | Description |
  |---|---|---|---|
  | `grant_type` | string | body | Authorization flow signature. Possible value is `client_credentials`. |
  | `client_id` | string | body |  |
  | `client_secret` | string | body |  |
  
  Response
  ```
  Status: 201 Created
  ```
  ```json
  {
    "access_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJjbGllbnRfaWQiOiI3Y2JkY2Q1Ni00ZDY1LTQxMjktYmU4My0zZWIzZTRhYjg4ODAiLCJleHAiOjE2MzcxNjIyNjYsImlhdCI6MTYzNjU1NzQ2Ni4wLCJpc3MiOiJvYXV0aC1zZXJ2aWNlIiwic3ViIjoib2F1dGgifQ.1SVmFC06sZb3cG3LvPCzjeoqLaQVv5Vd2hFL9G88OzY",
    "expires_in": 604800,
    "token_type": "Bearer"
  }
  ```

### Application commands
* `client` - manage OAuth clients:
  ```bash
  # list clients
  ./application client list
  
  # create client
  ./application client new --id=5f3a74e4-d25b-434f-9018-93cf10738099
  
  # delete client
  ./application client delete --id=5f3a74e4-d25b-434f-9018-93cf10738099
  
  # regenerate client's secret key
  ./application client update --id=5f3a74e4-d25b-434f-9018-93cf10738099
  
  # get client subcommand usage info
  ./application client [subcommand] --help
  ```
* `migrate` - apply and rollback database migrations:
  ```bash
  # apply migrations
  ./application migrate --database=default --migration=002_hello_world
  
  # rollback migrations
  ./application migrate --rollback --database=default --migration=001_initial
  ```
* `start-server` - start a web application:
  ```bash
  ./application start-server --bind=127.0.0.1:8000 --timeout-seconds=5
  ```

For more information about application commands, run:
```bash
# list all available commands
./application

# get command usage info
./application [command] --help
```
