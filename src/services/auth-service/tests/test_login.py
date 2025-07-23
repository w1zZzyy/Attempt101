import aiohttp
from .handler import Handler

async def test_login(service_client):
    h = Handler (service_client)

    response = await h.TryToRegister("test_login", "12345678")
    assert response.status == 200

    response = await h.TryToLogin("test_login_not", "12345678")
    assert response.status == 404

    response = await h.TryToLogin("test_login", "12345678")
    assert response.status == 200
