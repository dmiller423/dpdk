# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) 2010-2014 Intel Corporation
# Copyright(c) 2022-2023 PANTHEON.tech s.r.o.
# Copyright(c) 2022-2023 University of New Hampshire

"""
User-defined exceptions used across the framework.
"""

from enum import IntEnum, unique
from typing import ClassVar


@unique
class ErrorSeverity(IntEnum):
    """
    The severity of errors that occur during DTS execution.
    All exceptions are caught and the most severe error is used as return code.
    """

    NO_ERR = 0
    GENERIC_ERR = 1
    CONFIG_ERR = 2
    SSH_ERR = 3


class DTSError(Exception):
    """
    The base exception from which all DTS exceptions are derived.
    Stores error severity.
    """

    severity: ClassVar[ErrorSeverity] = ErrorSeverity.GENERIC_ERR


class SSHTimeoutError(DTSError):
    """
    Command execution timeout.
    """

    command: str
    output: str
    severity: ClassVar[ErrorSeverity] = ErrorSeverity.SSH_ERR

    def __init__(self, command: str, output: str):
        self.command = command
        self.output = output

    def __str__(self) -> str:
        return f"TIMEOUT on {self.command}"

    def get_output(self) -> str:
        return self.output


class SSHConnectionError(DTSError):
    """
    SSH connection error.
    """

    host: str
    severity: ClassVar[ErrorSeverity] = ErrorSeverity.SSH_ERR

    def __init__(self, host: str):
        self.host = host

    def __str__(self) -> str:
        return f"Error trying to connect with {self.host}"


class SSHSessionDeadError(DTSError):
    """
    SSH session is not alive.
    It can no longer be used.
    """

    host: str
    severity: ClassVar[ErrorSeverity] = ErrorSeverity.SSH_ERR

    def __init__(self, host: str):
        self.host = host

    def __str__(self) -> str:
        return f"SSH session with {self.host} has died"


class ConfigurationError(DTSError):
    """
    Raised when an invalid configuration is encountered.
    """

    severity: ClassVar[ErrorSeverity] = ErrorSeverity.CONFIG_ERR
