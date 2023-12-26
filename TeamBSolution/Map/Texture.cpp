#include "pch.h"
#include "Texture.h"


Texture::Texture() : ResourceBase(ResourceType::Texture)				// 상속받은 RB의 생성자를 호출
{
}

Texture::~Texture()
{
}

bool Texture::Load(const wstring& path)
{
	this->path = path;

	ComPtr<ID3D11Texture2D> texture;
	auto imageobj = ::make_shared<::ScratchImage>();
	::TexMetadata mdata;

	// load dds Texture file. 일반화된 코드
	HRESULT hr = ::GetMetadataFromDDSFile(path.c_str(), ::DDS_FLAGS_NONE, mdata); // 여기의 mdata는 왜 포인터를 전달하지 않는?
	if (SUCCEEDED(hr))
	{
		hr = ::LoadFromDDSFile(path.c_str(), ::DDS_FLAGS_NONE, &mdata, *imageobj); // 포인터로 받음
		if (SUCCEEDED(hr))
		{
			hr = ::CreateShaderResourceViewEx(LGlobal::g_pDevice.Get(), imageobj->GetImages(), imageobj->GetImageCount(),
												mdata, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0,
												CREATETEX_DEFAULT, shaderResourceView.ReleaseAndGetAddressOf());
			if (SUCCEEDED(hr))
			{
				size.x = mdata.width;
				size.y = mdata.height;
				return true;
			}
			else
			{
				Utils::ShowErrorMessage(hr);
				return false;
			}
		}
		else
		{
			Utils::ShowErrorMessage(hr);
			return false;
		}
	}



    // load png, jpg, etc Texture file
    hr = DirectX::GetMetadataFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, mdata);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, &mdata, *imageobj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceViewEx(LGlobal::g_pDevice.Get(), imageobj->GetImages(), imageobj->GetImageCount()
                , mdata, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0
                , CREATETEX_DEFAULT, shaderResourceView.ReleaseAndGetAddressOf());
            if (SUCCEEDED(hr))
            {
                size.x = mdata.width;
                size.y = mdata.height;
                return true;
            }
            else
            {
                Utils::ShowErrorMessage(hr);
                return false;
            }
        }
        else
        {
            Utils::ShowErrorMessage(hr);
            return false;
        }
    }

    // load tga Texture file
    hr = DirectX::GetMetadataFromTGAFile(path.c_str(), DirectX::TGA_FLAGS_NONE, mdata);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromTGAFile(path.c_str(), DirectX::TGA_FLAGS_NONE, &mdata, *imageobj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceViewEx(LGlobal::g_pDevice.Get(), imageobj->GetImages(), imageobj->GetImageCount()
                , mdata, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0
                , CREATETEX_DEFAULT, shaderResourceView.ReleaseAndGetAddressOf());
            if (SUCCEEDED(hr))
            {
                size.x = mdata.width;
                size.y = mdata.height;
                return true;
            }
            else
            {
                Utils::ShowErrorMessage(hr);
                return false;
            }
        }
        else
        {
            Utils::ShowErrorMessage(hr);
            return false;
        }
    }

	return false;
}

const shared_ptr<::ScratchImage> Texture::GetInfo()
{
    ComPtr<ID3D11Texture2D> texture;
    shaderResourceView->GetResource((ID3D11Resource**)texture.ReleaseAndGetAddressOf());
    
    shared_ptr<::ScratchImage> image = ::make_shared<::ScratchImage>();
    HRESULT hr = ::CaptureTexture(LGlobal::g_pDevice.Get(), LGlobal::g_pImmediateContext.Get(), texture.Get(), *image.get());
    
    if (FAILED(hr))
        Utils::ShowErrorMessage(hr);

	return image;
}

void Texture::CreateTexture(int width, int height)
{
    D3D11_TEXTURE2D_DESC desc;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&desc, sizeof(desc));
    ZeroMemory(&data, sizeof(data));
    
    BYTE* buf = new BYTE[width * height * 4];
    for (int i = 0; i < width * height * 4; i += 4)
    {
        buf[i + 0] = 0;
        buf[i + 1] = 0;
        buf[i + 2] = 0;
        buf[i + 3] = 0;     // 
    }

    data.pSysMem = (void*)buf;
    data.SysMemPitch = width * 4;
    data.SysMemSlicePitch = 0;
    
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.MiscFlags = 0;

    size.x = width;
    size.y = height;

    ComPtr<ID3D11Texture2D> texture;
    HRESULT hr = LGlobal::g_pDevice->CreateTexture2D(&desc, &data, texture.ReleaseAndGetAddressOf());

    if (FAILED(hr))
        Utils::ShowErrorMessage(hr);
    hr = LGlobal::g_pDevice->CreateShaderResourceView(texture.Get(), nullptr, shaderResourceView.ReleaseAndGetAddressOf());

    if (FAILED(hr))
        Utils::ShowErrorMessage(hr);
    delete[] buf;
}

void Texture::UpdateTexture(const vector<BYTE>& colors)
{
    ComPtr<ID3D11Texture2D> texture;
    shaderResourceView->GetResource((ID3D11Resource**)texture.ReleaseAndGetAddressOf());
    
    D3D11_TEXTURE2D_DESC desc;
    texture->GetDesc(&desc);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(mappedResource));
    
    HRESULT hr = LGlobal::g_pImmediateContext->Map(texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr))
        Utils::ShowErrorMessage(hr);

    BYTE* mappedData = reinterpret_cast<BYTE*>(mappedResource.pData);
    
    for (int i = 0; i < size.x; i++)
    {
        UINT rowStart = i * mappedResource.RowPitch;
        UINT colorRowStart = i * size.x * 4;
        int k = 0;
        for (int j = 0; j < size.y; ++j)
        {
            UINT colStart = j * 4;
            UINT index = rowStart + colStart;
            
            mappedData[index + 0] = colors[colorRowStart + k];
            mappedData[index + 1] = colors[colorRowStart + k + 1];
            mappedData[index + 2] = colors[colorRowStart + k + 2];
            mappedData[index + 3] = colors[colorRowStart + k + 3];

            k += 4;
        }
    }


    LGlobal::g_pImmediateContext->Unmap(texture.Get(), 0);
}

void Texture::SaveTexture(const wstring& path)
{
    shared_ptr<::ScratchImage> srcimage = GetInfo();
    ::SaveToWICFile(srcimage->GetImages(), srcimage->GetImageCount(),
        ::WIC_FLAGS_NONE, ::GetWICCodec(::WIC_CODEC_PNG),
        path.c_str());
}

void Texture::GetTextureRGBAData(vector<BYTE>& colors)
{
    auto& info = GetInfo();
    auto mData = info->GetMetadata();
    auto images = info->GetImages();

    UINT rowNum = mData.height;
    UINT colNum = mData.width;

    colors.resize(rowNum * colNum * 4);
    BYTE* pTexels = (BYTE*)images->pixels;

    for (UINT i = 0; i < rowNum; i++)
    {
        UINT rowStart = i * images->rowPitch;
        UINT colorRowStart = i * 4 * mData.width;
        for (UINT j = 0; j < colNum; j++)
        {
            UINT colStart = j * 4;
            
            colors[colorRowStart + colStart + 0] = pTexels[rowStart + colStart + 0];
            colors[colorRowStart + colStart + 1] = pTexels[rowStart + colStart + 1];
            colors[colorRowStart + colStart + 2] = pTexels[rowStart + colStart + 2];
            colors[colorRowStart + colStart + 3] = pTexels[rowStart + colStart + 3];
        }
    }
}
