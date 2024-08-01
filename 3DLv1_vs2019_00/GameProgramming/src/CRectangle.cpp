#include "CRectangle.h"
#include "glut.h"

CRectangle::CRectangle()
{

}

CRectangle::~CRectangle()
{

}

void CRectangle::Render()
{
	//éläpå`ÇÃï`é 
	glBegin(GL_QUADS);
	glVertex2f(mX - mW, mY - mH);
	glVertex2f(mX + mW, mY - mH);
	glVertex2f(mX + mW, mY + mH);
	glVertex2f(mX - mW, mY + mH);
	glEnd();
}

void CRectangle::Render(float x, float y, float w, float h)
{
	glBegin(GL_QUADS);
	glVertex2f(mX - mW, mY - mH);
	glVertex2f(mX + mW, mY - mH);
	glVertex2f(mX + mW, mY + mH);
	glVertex2f(mX - mW, mY + mH);
}

void CRectangle::Set(float x, float y, float w, float h)
{
	mX = x;
	mY = y;
	mW = w;
	mH = h;
}

float CRectangle::GetX()
{
	return mX;
}

float CRectangle::GetY()
{
	return mY;
}

float CRectangle::GetZ()
{
	return mZ;
}

float CRectangle::GetW()
{
	return mW;
}

float CRectangle::GetH()
{
	return mH;
}

void CRectangle::SetY(float y)
{
	mY = y;
}

void CRectangle::SetX(float x)
{
	mX = x;
}

void CRectangle::SetZ(float z)
{
	mZ = z;
}

void CRectangle::SetW(float w)
{
	mW = w;
}

void CRectangle::SetH(float h)
{
	mH = h;
}