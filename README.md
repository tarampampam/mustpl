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
- Powerful templating engine under the hood - {{ [mustache][mustache] }}
- Distributed as compiled for many architectures, including **docker** image
- Lightweight _(**~970Kb** statically linked, amd64)_ and fast (written in pure C)
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
| [Docker Hub][docker-hub]          | `tarampampam/mustpl`         |
| [GitHub Container Registry][ghcr] | `ghcr.io/tarampampam/mustpl` |

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

COPY --from=ghcr.io/tarampampam/mustpl:latest /bin/mustpl /bin/mustpl

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

> TODO: TBD

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
