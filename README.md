<div align="center">

[![banner][banner]][repo]

[![badge-version][badge-version]][release-latest]
[![badge-tests][badge-tests]][actions]
[![badge-release][badge-release]][actions]
[![badge-image-size][badge-image-size]][actions]
[![badge-license][badge-license]][docker-hub]

[banner]:https://user-images.githubusercontent.com/7326800/185954094-bdc16f92-4554-4e41-97e9-82e5b9e47b53.png
[badge-tests]:https://img.shields.io/github/workflow/status/tarampampam/mustpl/tests?maxAge=30&label=tests&logo=github&style=flat-square
[badge-release]:https://img.shields.io/github/workflow/status/tarampampam/mustpl/release?maxAge=30&label=release&logo=github&style=flat-square
[badge-image-size]:https://img.shields.io/docker/image-size/tarampampam/mustpl/latest?maxAge=30&label=size&logo=docker&logoColor=white&style=flat-square
[badge-license]:https://img.shields.io/github/license/tarampampam/mustpl.svg?maxAge=30&style=flat-square
[badge-version]:https://img.shields.io/github/v/release/tarampampam/mustpl?maxAge=30&label=version&style=flat-square
</div>

# 🧾 Overview

Sometimes you may need to generate data using templates, and this tool allows you to do it in the simplest way. All it takes is the template itself, the data for it (the values that are inserted in the template), and this tool.

<details>
  <summary>👉 The simplest example</summary>

Let's imagine, that you have such a Nginx template:

```nginx
# File `nginx.tpl`

server {
  listen      {{ port }};
  server_name {{ server_name }};

  location / {
    root  /usr/share/nginx/html;
    index index.html index.htm;
  }
}
```

All that is required for its rendering is:

```shell
$ PORT_NUM="8080"
$ mustpl -d '{"port": "${PORT_NUM:-8888}", "server_name": "example.com"}' ./nginx.tpl

server {
  listen      8080;
  server_name example.com;

  location / {
    root  /usr/share/nginx/html;
    index index.html index.htm;
  }
}
```

</details>

## 🔥 Features list

- Has zero external dependencies
- Can be used in a docker `scratch` image (empty file system)
- Powerful templating engine under the hood - {{ [mustache][mustache] }}
- Distributed as compiled for many architectures, including **docker** image
- Lightweight _(**~400Kb** compressed, statically linked)_ and fast (written in pure C)
- Support environment variables in the template data, with default values (`${ENV_NAME:-default value}`)
- Can be used as docker entrypoint (can start another application without PID changing - `mustpl ... -- nginx -g 'daemon off;'`)

## 🧩 Install

Download the latest binary file for your arch (_only the linux-like platforms are supported at the moment_) from the [releases page][release-latest]. For example, let's install it on amd64 arch (e.g.: Debian, Ubuntu, etc):

```bash
$ curl -SsL -o ./mustpl https://github.com/tarampampam/mustpl/releases/latest/download/mustpl-linux-amd64
$ chmod +x ./mustpl

# optionally, install the binary file globally:
$ sudo install -g root -o root -t /usr/local/bin -v ./mustpl
$ rm ./mustpl
$ mustpl --help
```

### 🛸 Compilation from sources

All you need to compile - is gcc:

```shell
$ sudo apt install gcc
```

Get the sources and switch to the latest version:

```shell
$ git clone https://github.com/tarampampam/mustpl.git ./mustpl
$ cd ./mustpl
$ git fetch --tags
$ git checkout $(git describe --tags `git rev-list --tags --max-count=1`)
```

And compile:

```shell
$ make version=1.1.1 # set your version
$ ./mustpl --help
```

### 🐋 Docker

Additionally, you can use our docker image:

| Registry                          | Image                        |
|-----------------------------------|------------------------------|
| [GitHub Container Registry][ghcr] | `ghcr.io/tarampampam/mustpl` |
| [Docker Hub][docker-hub]          | `tarampampam/mustpl`         |

> Using the `latest` tag for the docker image is highly discouraged because of possible backward-incompatible changes during **major** upgrades. Please, use tags in `X.Y.Z` format

<details>
  <summary>What's inside the Docker image?</summary>

To watch the docker image content you can use the [dive](https://github.com/wagoodman/dive):

```shell
$ docker run --rm -it \
    -v "/var/run/docker.sock:/var/run/docker.sock:ro" \
    wagoodman/dive:latest \
      ghcr.io/tarampampam/mustpl:latest
```

<div align="center">

![dive](https://user-images.githubusercontent.com/7326800/186013969-743634e4-3263-42c6-82f4-2283e9f8755e.png)

</div>

</details>

<details>
  <summary>Which platforms are supported?</summary>

Following platforms for this image are available:

```shell
$ docker run --rm mplatform/mquery ghcr.io/tarampampam/mustpl:latest
Image: tarampampam/mustpl:latest
 * Manifest List: Yes
 * Supported platforms:
   - linux/386
   - linux/amd64
   - linux/arm/v5
   - linux/arm/v7
   - linux/arm64
   - linux/mips64le
   - linux/ppc64le
   - linux/s390x
```

</details>

To run locally:

```shell
$ docker run --rm -ti \
    -u "$(id -u):$(id -g)" \
    -v "$(pwd):/rootfs:rw" \
    -w /rootfs \
      ghcr.io/tarampampam/mustpl --help
```

Or install it into another image:

```dockerfile
FROM nginx:alpine

COPY --from=ghcr.io/tarampampam/mustpl /bin/mustpl /bin/mustpl

RUN mustpl --help
```

## 🛠 Usage

```shell
Usage: mustpl [OPTION...] <template-file> [-- <exec-command>]

{{ mustach }} templating tool. For more details about the templating engine and
rules, please, follow this link: https://mustache.github.io/mustache.5.html

You can use environment variables in your template data using the following
format: ${ENV_NAME} or ${ENV_NAME:-default value}. Only those formats are
allowed (not $ENV_NAME).

 Template data:
  -d, --data=<json-string>   Template data in JSON-string format (has higher
                             priority than --data-file flag)
  -f, --data-file=<file>     Read template data from the JSON file

 Output:
  -o, --out=<out-file>       Write output to the file instead of standard
                             output

  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
```

### 🦾 What the `mustpl` can do

> For detailed information about the templating engine please refer to the following link: [mustache manual][mustache]

For example, you have the following template data (`data.json`):

```json
{
  "name": "Chris",
  "value": 10000,
  "taxed_value": 6000,
  "in_ca": true
}
```

And template (`template.txt`):

```mustache
Hello {{name}}
You have just won {{value}} dollars!
{{#in_ca}}
Well, {{taxed_value}} dollars, after taxes.
{{/in_ca}}
```

Let's do the magic!

```shell
$ mustpl -f ./data.json ./template.txt
Hello Chris
You have just won 10000 dollars!
Well, 6000 dollars, after taxes.
```

#### Loops

Okay, but what about the **loops**? Let's see:

```json
{
  "servers": [
    {
      "listen": 8080,
      "names": [
        {"name": "example.com"}
      ],
      "is_default": true,
      "home": "/www/example.com"
    },
    {
      "listen": 1088,
      "names": [
        {"name": "127-0-0-1.nip.io"},
        {"name": "127-0-0-2.nip.io"}
      ],
      "home": "/www/local"
    }
  ]
}
```

```mustache
{{#servers}}
server { {{! just a comment, will not be rendered }}
  listen      {{ listen }};
  server_name{{#names}} {{ name }}{{/names}}{{#is_default}} default_server{{/is_default}};

  location / {
    root  {{ home }};
    index index.html index.htm;
  }
}
{{/servers}}
```

```shell
$ mustpl -f ./data.json ./template.txt
server {
  listen      8080;
  server_name example.com default_server;

  location / {
    root  /www/example.com;
    index index.html index.htm;
  }
}
server {
  listen      1088;
  server_name 127-0-0-1.nip.io 127-0-0-2.nip.io;

  location / {
    root  /www/local;
    index index.html index.htm;
  }
}
```

#### Template data providing using options

You can provide your template data using the `-d` (`--data`) option:

```mustache
server {
  listen      8080;
  server_name{{#names}} {{ name }}{{/names}};

  location / {
    root  /var/www/data;
    index index.html index.htm;
  }
}
```

```shell
$ mustpl -d '{"names": [{"name": "example.com"}, {"name": "google.com"}]}' ./template.txt
server {
  listen      8080;
  server_name example.com google.com;

  location / {
    root  /var/www/data;
    index index.html index.htm;
  }
}
```

#### Environment variables

Environment variables can be used in the format `${ENV_NAME:-default value}` (inside template data file too; _template from the example above is used_):

```shell
$ SERVER_NAME_1=example.com ./mustpl -d '{"names": [{"name": "${SERVER_NAME_1:-fallback.com}"}, {"name": "${SERVER_NAME_X:-unset.com}"}]}' ./tmp/template.txt
server {
  listen      8080;
  server_name example.com unset.com;

  location / {
    root  /var/www/data;
    index index.html index.htm;
  }
}
```

#### `exec` and the PID magic

As you probably know, the main process inside the docker container should have PID == 1 for the correct signals processing from the docker daemon. That's why basically entrypoint scripts have the following code:

```shell
#!/bin/sh
set -e

if [ -n "$MY_OPTION" ]; then
  sed -i "s~foo~bar ${MY_OPTION}~" /etc/app.cfg # modify the config
fi;

exec "$@" # <-- that's it!
```

```dockerfile
# ...

COPY docker-entrypoint.sh ./docker-entrypoint.sh

ENTRYPOINT ["/docker-entrypoint.sh"]

CMD ["/bin/app", "--some", "option"]
```

From the `man exec`:

> The `exec()` family  of functions replaces the current process image with a new process image

So, the application `/bin/app` will have a PID == 1 that previously was assigned to the `bash` (because the `bash` is the image entrypoint). `mustpl` uses the same technique, let's create the following files for the example:

`data.json`:

```json
{
  "my_option": "${MY_OPTION:-default value}"
}
```

`template.txt`:

```mustache
Hello {{ my_option }}!
```

`Dockerfile`:

```dockerfile
FROM alpine:latest

COPY --from=ghcr.io/tarampampam/mustpl /bin/mustpl /bin/mustpl

COPY ./data.json /data.json
COPY ./template.txt /template.txt

ENTRYPOINT ["mustpl", "-f", "/data.json", "-o", "/rendered.txt", "/template.txt", "--"]

CMD ["sleep", "infinity"]
```

Next, build the image:

```shell
$ docker build --tag test:local .
```

And run it:

```shell
$ docker run --rm --name mustpl_example -e "MY_OPTION=foobar" test:local
```

In the separate terminal run:

```shell
$ docker exec mustpl_example ps aux
PID   USER     TIME  COMMAND
    1 root      0:00 sleep infinity # <-- our CMD with a PID == 1
    7 root      0:00 ps aux

$ docker exec mustpl_example cat /rendered.txt
Hello foobar! # <-- our environment variable value

$ docker kill mustpl_example
```

Agree that this approach is easier than using `sed`, `awk`, and other tools to modify the configuration files before running the main application. But despite this, no one is restricting you from using the entrypoint scripts 😉

## 🔌 Used libraries

- [`cJSON` - Ultralightweight JSON parser in ANSI C](https://github.com/DaveGamble/cJSON/)
- [`mustach` - C implementation of mustache templating](https://gitlab.com/jobol/mustach)

## 📰 Changes log

[![Release date][badge-release-date]][releases]
[![Commits since latest release][badge-commits]][commits]

Changes log can be [found here][changelog].

## 👾 Support

[![Issues][badge-issues]][issues]
[![Issues][badge-prs]][prs]

If you find any bugs in the project, please [create an issue][new-issue] in the current repository.

## 📖 License

This is open-sourced software licensed under the [MIT License][license].

[badge-release-date]:https://img.shields.io/github/release-date/tarampampam/mustpl.svg?maxAge=180
[badge-commits]:https://img.shields.io/github/commits-since/tarampampam/mustpl/latest.svg?maxAge=45
[badge-issues]:https://img.shields.io/github/issues/tarampampam/mustpl.svg?maxAge=45
[badge-prs]:https://img.shields.io/github/issues-pr/tarampampam/mustpl.svg?maxAge=45

[repo]:https://github.com/tarampampam/mustpl
[docker-hub]:https://hub.docker.com/r/tarampampam/mustpl
[ghcr]:https://github.com/users/tarampampam/packages/container/package/mustpl
[actions]:https://github.com/tarampampam/mustpl/actions
[releases]:https://github.com/tarampampam/mustpl/releases
[release-latest]:https://github.com/tarampampam/mustpl/releases/latest
[commits]:https://github.com/tarampampam/mustpl/commits
[issues]:https://github.com/tarampampam/mustpl/issues
[new-issue]:https://github.com/tarampampam/mustpl/issues/new/choose
[prs]:https://github.com/tarampampam/mustpl/pulls
[changelog]:https://github.com/tarampampam/mustpl/blob/master/CHANGELOG.md
[license]:https://github.com/tarampampam/mustpl/blob/master/LICENSE

[mustache]:https://mustache.github.io/
