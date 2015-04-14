﻿// ------------------------------------------------------------------------------
//  <autogenerated>
//      This code was generated by a tool.
//      Mono Runtime Version: 4.0.30319.1
// 
//      Changes to this file may cause incorrect behavior and will be lost if 
//      the code is regenerated.
//  </autogenerated>
// ------------------------------------------------------------------------------
using System;

namespace FuzzyLogicSolver
{
	public class Centroid : IntegralDefuzzyfier
	{
		public override float defuzzify ( Accumulated term, float minimum, float maximum )
		{
			float dx = (maximum - minimum) / resolution;
			float x, y;
			float area = 0, xcentroid = 0, ycentroid = 0;
			for (int i = 0; i < resolution; i ++) {
				x = minimum + (i+0.5f)*dx;
				y = term.Membership(x);
				xcentroid+=y*x;
				ycentroid+=y*y;
				area+=y;
			}
			xcentroid /= area;
			ycentroid /= area;
			area *= dx;
			return xcentroid;
		}
		protected Centroid (int divisions) : base ( divisions )
		{
		}
		public static Defuzzyfier Builder (params string[] arguments) {
			int a = 0;
			if ( arguments.Length > 0 ) a = Int32.Parse (arguments [0], System.Globalization.CultureInfo.InvariantCulture);
			return new Centroid (a);
		}
	}
}