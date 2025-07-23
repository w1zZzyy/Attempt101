import pytest
from .handler import Handler

async def test_register(service_client):
    h = Handler (service_client)

    response = await h.TryToRegister("test_register", "12345678")
    assert response.status == 200

    response = await h.TryToRegister("test_register", "12345678")
    assert response.status == 409

async def test_bad_req(service_client):
    h = Handler (service_client)

    response = await h.TryToRegister("", "1234567")
    assert response.status == 400

    response = await h.TryToRegister("x", "1234567")
    assert response.status == 400