import aiohttp

def create_request(name, password):
    with aiohttp.MultipartWriter('form-data') as data:
        name_payload = aiohttp.payload.StringPayload(name)
        name_payload.set_content_disposition('form-data', name='name')

        password_payload = aiohttp.payload.StringPayload(password)
        password_payload.set_content_disposition('form-data', name='password')

        data.append_payload(name_payload)
        data.append_payload(password_payload)
    
    headers = {
        'Content-Type': 'multipart/form-data; boundary=' + data.boundary,
    }

    return (data, headers)


class Handler:
    def __init__(self, service_client):
        self.service_client = service_client

    async def TryToRegister(self, name, password):
        data, headers = create_request(name, password)

        response = await self.service_client.post(
            'auth/register',
            data=data,
            headers=headers
        )

        return response

    async def TryToLogin(self, name, password):
        data, headers = create_request(name, password)

        response = await self.service_client.post(
            'auth/login',
            data=data,
            headers=headers
        )

        return response
