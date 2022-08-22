<div align="center">

[![banner][banner]][repo]

[![badge-tests][badge-tests]][actions]
[![badge-release][badge-release]][actions]
![badge-license][badge-license]

[banner]:https://user-images.githubusercontent.com/7326800/185954094-bdc16f92-4554-4e41-97e9-82e5b9e47b53.png
[badge-tests]:https://img.shields.io/github/workflow/status/tarampampam/mustpl/tests?maxAge=30&label=tests&logo=github&style=flat-square
[badge-release]:https://img.shields.io/github/workflow/status/tarampampam/mustpl/release?maxAge=30&label=release&logo=github&style=flat-square
[badge-license]:https://img.shields.io/github/license/tarampampam/mustpl.svg?maxAge=30&style=flat-square
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

- Powerful templating engine under the hood - {{ [mustache] }}
- Has zero external dependencies
- Extremely lightweight _(~47Kb dynamically or ~970Kb statically compiled)_ and fast (written in pure C)
- Distributed both **statically** and dynamically compiled for many architectures, including **docker** image
- Support environment variables in the template data, with default values (`${ENV_NAME:-default value}`)
- Can be used as docker entrypoint (can start another application without PID changing - `mustpl ... -- nginx -g 'daemon off;'`)

## 🧩 Install

> TODO: TBD

## 🛸 Compilation

> TODO: TBD

```shell
$ make # dynamic linking
$ make static=yes # static linking
$ make version=1.1.1 # set the version
```

## 🛠 Usage

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
[actions]:https://github.com/tarampampam/mustpl/actions
[releases]:https://github.com/tarampampam/mustpl/releases
[commits]:https://github.com/tarampampam/mustpl/commits
[issues]:https://github.com/tarampampam/mustpl/issues
[new-issue]:https://github.com/tarampampam/mustpl/issues/new/choose
[prs]:https://github.com/tarampampam/mustpl/pulls
[changelog]:https://github.com/tarampampam/mustpl/blob/master/CHANGELOG.md
[license]:https://github.com/tarampampam/mustpl/blob/master/LICENSE

[mustache]:https://mustache.github.io/
