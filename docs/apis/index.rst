Kubos APIs
==========

The Kubos SDK provides a variety of APIs to help with the development of mission software:

  - :doc:`Device Interfaces <device-api/index>` - APIs for external devices (ex. radio), built on top of the Kubos HAL
  - :doc:`OBC APIs <obc-api/index>` - APIs for features which are internal to a particular OBC
  - :doc:`Kubos HAL <kubos-hal/index>` - Hardware interface abstractions (I2C, SPI, etc)

Some third-party APIs are also included with the Kubos SDK:

  - :doc:`CSP (Cubesat Space Protocol) <libcsp/index>` - A small protocol stack targeting embedded systems

.. toctree::
    :caption: APIs
    :hidden:
    
    Device Interfaces <device-api/index>
    OBC APIs <obc-api/index>
    Kubos HAL <kubos-hal/index>

    CSP <libcsp/index>