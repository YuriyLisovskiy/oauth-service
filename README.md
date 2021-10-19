## oauth service
OAuth service.

### Application commands
* `client` - manage OAuth clients:
  ```bash
  # list clients
  ./application client
  
  # create client
  ./application client --new --id=5f3a74e4-d25b-434f-9018-93cf10738099
  
  # delete client
  ./application client --delete --id=5f3a74e4-d25b-434f-9018-93cf10738099
  
  # edit client
  ./application client --edit --id=5f3a74e4-d25b-434f-9018-93cf10738099 --secret=super_256_secret_512
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
