function [ point, u, v ] = IntersectRaySurface( n,p,U,m,q,V,ctrlX, ctrlY, ctrlZ, linePoint, dir )
%计算直线linePoine, dir与B样条曲面的交点

uMin = 0;
vMin = 0;
uMax = 1;
vMax = 1;

counter = 0;
dmin = DistanceToLine(linePoint, dir,...
                      [SurfacePoint(n, p, U, m, q, V, ctrlX, uMin, vMin), ...
                       SurfacePoint(n, p, U, m, q, V, ctrlY, uMin, vMin), ...
                       SurfacePoint(n, p, U, m, q, V, ctrlZ, uMin, vMin)]);
while (dmin > 0.001 && counter < 30 && uMax - uMin > 0.00001 && vMax - vMin > 0.00001)
    counter = counter + 1;
    u1 = (uMin + 0.25 * (uMax - uMin));
    v1 = (vMin + 0.25 * (vMax - vMin));
    d1 = DistanceToLine(linePoint, dir,...
                      [SurfacePoint(n, p, U, m, q, V, ctrlX, u1, v1), ...
                       SurfacePoint(n, p, U, m, q, V, ctrlY, u1, v1), ...
                       SurfacePoint(n, p, U, m, q, V, ctrlZ, u1, v1)]);
    u2 = (uMin + 0.75 * (uMax - uMin));
    v2 = (vMin + 0.25 * (vMax - vMin));
    d2 = DistanceToLine(linePoint, dir,...
                      [SurfacePoint(n, p, U, m, q, V, ctrlX, u2, v2), ...
                       SurfacePoint(n, p, U, m, q, V, ctrlY, u2, v2), ...
                       SurfacePoint(n, p, U, m, q, V, ctrlZ, u2, v2)]);   
    u3 = (uMin + 0.75 * (uMax - uMin));
    v3 = (vMin + 0.75 * (vMax - vMin));
    d3 = DistanceToLine(linePoint, dir,...
                      [SurfacePoint(n, p, U, m, q, V, ctrlX, u3, v3), ...
                       SurfacePoint(n, p, U, m, q, V, ctrlY, u3, v3), ...
                       SurfacePoint(n, p, U, m, q, V, ctrlZ, u3, v3)]);
    u4 = (uMin + 0.25 * (uMax - uMin));
    v4 = (vMin + 0.75 * (vMax - vMin));
    d4 = DistanceToLine(linePoint, dir,...
                      [SurfacePoint(n, p, U, m, q, V, ctrlX, u4, v4), ...
                       SurfacePoint(n, p, U, m, q, V, ctrlY, u4, v4), ...
                       SurfacePoint(n, p, U, m, q, V, ctrlZ, u4, v4)]);
    if (d1 <= d2 && d1 <= d3 && d1 <= d4)
        uMax = (uMax + uMin) / 2;
        vMax = (vMax + vMin) / 2;
        dmin = d1;
    elseif (d2 <= d1 && d2 <= d3 && d2 <= d4)
        uMin = (uMax + uMin) / 2;
        vMax = (vMax + vMin) / 2;
        dmin = d2;
    elseif (d3 <= d1 && d3 <= d2 && d3 <= d4)
        uMin = (uMax + uMin) / 2;
        vMin = (vMax + vMin) / 2;
        dmin = d3;
    else
        vMin = (vMax + vMin) / 2;
        uMax = (uMin + uMax) / 2;
        dmin = d4;
    end
end
    u = (uMin + uMax) / 2;
    v = (vMin + vMax) / 2;
    point = [SurfacePoint(n, p, U, m, q, V, ctrlX, u, v),...
                SurfacePoint(n, p, U, m, q, V, ctrlY, u, v),...
                SurfacePoint(n, p, U, m, q, V, ctrlZ, u, v)];    
end

