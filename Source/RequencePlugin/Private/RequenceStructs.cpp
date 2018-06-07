// Fill out your copyright notice in the Description page of Project Settings.

#include "RequenceStructs.h"

URequenceStructs::URequenceStructs()
{
}

float URequenceStructs::Interpolate(TArray<FVector2D> Points, float val)
{
	TArray<FVector2D> Data;

	//Build new data array, first -1, then flip points and add them as negative. add zero, add points, add 1.
	Data.Add(FVector2D(-1, -1));
	for (int i = Points.Num(); i >= 0; i--) { Data.Add(FVector2D(Points[i].X * -1, Points[i].Y * -1)); }
	Data.Add(FVector2D::ZeroVector);
	for (FVector2D vec : Points) { Data.Add(vec); }
	Data.Add(FVector2D(1, 1));

	for (int i = 1; i < Data.Num(); i++)	//Note, skips first
	{
		if (Points[i].X < val) { continue; }

		float calc = Points[i - 1].Y + (val - Points[i - 1].X) * ((Points[i].Y - Points[i - 1].Y) / (Points[i].X - Points[i - 1].X));
		return calc;
	}

	return val;
}
