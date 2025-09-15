#!/usr/bin/env python3
import subprocess
import sys
from enum import StrEnum
from pathlib import Path
from typing import Annotated, TypeAlias

import typer

app = typer.Typer()

CMAKE_ARGS = """-G "Ninja Multi-Config" --fresh"""
BUILD_DIR = "build"
SOURCE_DIR = "."  # adjust if your source is elsewhere
EXECUTABLE = Path(".").resolve().name


class Config(StrEnum):
    DEBUG = "Debug"
    RELEASE = "Release"


valid_names = ["Debug", "Release"]


def complete_name(incomplete: str):
    completion = []
    for name in valid_names:
        if name.startswith(incomplete):
            completion.append(name)
    return completion


ConfigType = Annotated[Config, typer.Option(help="Build configuration", autocompletion=complete_name)]


def run_cmd(cmd: str, cwd: str | Path | None = None, extra: str = ""):
    print(f"> {cmd}")
    res = subprocess.run(cmd, shell=True, cwd=cwd)
    if res.returncode != 0:
        sys.exit(res.returncode)


@app.command()
def cmake(extra: str = ""):
    """Run CMake configure"""
    Path(BUILD_DIR).mkdir(exist_ok=True)
    run_cmd(f"cmake {CMAKE_ARGS} -S {SOURCE_DIR} -B {BUILD_DIR} {extra}")


@app.command()
def build(config: ConfigType = Config.DEBUG, extra: str = ""):
    """Build the project"""
    run_cmd(f"cmake --build {BUILD_DIR} --target {EXECUTABLE} --config {config} -j 10 {extra}")


@app.command()
def execute(config: ConfigType = Config.DEBUG, extra: str = ""):
    """Run the executable"""
    exe_path = Path(BUILD_DIR) / config / EXECUTABLE
    if not exe_path.exists():
        typer.echo(f"Executable not found: {exe_path} {extra}")
        raise typer.Exit(code=1)
    run_cmd(exe_path.as_posix())


@app.command()
def all(config: ConfigType = Config.DEBUG):
    """Run full pipeline: cmake -> build -> run"""
    cmake()
    build(config=config)
    execute(config=config)

# if __name__ == "__main__":
#     app()
