using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class AreaDivideTest : MonoBehaviour
{
    public int AreaCount = 1;

    public GameObject AreaPointPrefab;
    private List<GameObject> planeObjs;
    private List<GameObject> planes;

    private List<int>[] planeIndicesList;

    Mesh[] meshes;
    Vector3[] vertices;
    int[] areas;
    int[] indices;

    Vector3[] tempVec3;
    int[] tempIndices;

    int currentIdx = 0;

    public Vector3 TerrainCenter = Vector3.zero;
    private Vector3[] terrainVertexes;
    private Terrain[] terrains;
    private float terrainWidth;
    private float terrainHeight;
    float minX;
    float maxX;
    float minZ;
    float maxZ;

    private void Awake()
    {
        terrains = FindObjectsOfType<Terrain>();

        FindTerrainVertexes();

        FindTerrainMinMaxXY();

        MakeArea();

    }

    void Start()
    {
        NavMeshTriangulation navMeshTriangulation = NavMesh.CalculateTriangulation();
        indices = navMeshTriangulation.indices;
        areas = navMeshTriangulation.areas;
        vertices = navMeshTriangulation.vertices;
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {
            InitializeArea();

            for (int i = 0; i < indices.Length - 3; i += 3)
            {
                InsertToShortestDistPlane(indices[i], indices[i + 1], indices[i + 2]);
            }

            ChangePlaneValue();
        }
    }

    void FindTerrainVertexes()
    {
        terrainVertexes = new Vector3[4];

        // 모든 터레인의 꼭지점을 구해서 터레인 집합의 중점을 구한다.
        terrainWidth = terrains[0].terrainData.size.x;
        terrainHeight = terrains[0].terrainData.size.z;

        // 꼭지점 4곳을 '전체 터레인이 회전하지 않음'을 가정하여 구함.
        if (terrains.Length % 2 == 0)
        {
            terrainWidth = (terrains.Length / 4) * terrainWidth;
            terrainHeight = (terrains.Length / 4) * terrainHeight;
        }
        else
        {
            terrainWidth = Mathf.Floor(terrains.Length / 4) * terrainWidth + terrainWidth;
            terrainHeight = Mathf.Floor(terrains.Length / 4) * terrainHeight + terrainHeight;
        }

        terrainVertexes[0] = new Vector3(TerrainCenter.x + terrainWidth * 0.5f, TerrainCenter.y, TerrainCenter.z + terrainHeight * 0.5f);
        terrainVertexes[1] = new Vector3(TerrainCenter.x + terrainWidth * 0.5f, TerrainCenter.y, TerrainCenter.z - terrainHeight * 0.5f);
        terrainVertexes[2] = new Vector3(TerrainCenter.x - terrainWidth * 0.5f, TerrainCenter.y, TerrainCenter.z - terrainHeight * 0.5f);
        terrainVertexes[3] = new Vector3(TerrainCenter.x - terrainWidth * 0.5f, TerrainCenter.y, TerrainCenter.z + terrainHeight * 0.5f);
    }

    void FindTerrainMinMaxXY()
    {
        minX = float.MaxValue;
        maxX = float.MinValue;
        minZ = float.MaxValue;
        maxZ = float.MinValue;
        for (int i = 0; i < terrainVertexes.Length; i++)
        {
            if (terrainVertexes[i].x < minX)
                minX = terrainVertexes[i].x;
            if (terrainVertexes[i].x > maxX)
                maxX = terrainVertexes[i].x;

            if (terrainVertexes[i].z < minZ)
                minZ = terrainVertexes[i].z;
            if (terrainVertexes[i].z > maxZ)
                maxZ = terrainVertexes[i].z;
        }
    }

    void MakeArea()
    {
        planeObjs = new List<GameObject>();
        planes = new List<GameObject>();

        meshes = new Mesh[AreaCount];
        for (int i = 0; i < meshes.Length; i++)
        {
            meshes[i] = new Mesh();
        }
        planeIndicesList = new List<int>[AreaCount];
        for (int i = 0; i < planeIndicesList.Length; i++)
        {
            planeIndicesList[i] = new List<int>();
        }
    }

    void InitializeArea()
    {
        planeObjs.Clear();
        planes.Clear();
        for (int i = 0; i < planeIndicesList.Length; i++)
        {
            planeIndicesList[i].Clear();
        }

        for (int i = 0; i < AreaCount; i++)
        {
            planeObjs.Add(Instantiate(AreaPointPrefab, new Vector3(Random.Range(minX, maxX), transform.position.y, Random.Range(minZ, maxZ)), transform.rotation));
            planeObjs[i].GetComponent<Renderer>().material.color = Random.ColorHSV();
            planes.Add(planeObjs[i]);
        }
    }

    void InsertToShortestDistPlane(int idx1, int idx2, int idx3)
    {
        // 삼각형 중심 계산
        Vector3 center = (vertices[idx1] + vertices[idx2] + vertices[idx3]) / 3;

        // 모든 플레인의 중심과 비교하여 중심과 가장 가까운 플레인 찾기
        int shortestIdx = 0; ;
        float shortestDist = float.MaxValue;
        for (int i = 0; i < AreaCount; i++)
        {
            if (Vector3.Distance(planes[i].transform.position, center) < shortestDist)
            {
                shortestDist = Vector3.Distance(planes[i].transform.position, center);
                shortestIdx = i;
            }
        }
        // 해당 플레인에 삼각형 꼭지점 인덱스 삽입
        planeIndicesList[shortestIdx].Add(idx1);
        planeIndicesList[shortestIdx].Add(idx2);
        planeIndicesList[shortestIdx].Add(idx3);
        
    }

    void ChangePlaneValue()
    {
        for (int i = 0; i < AreaCount; i++)
        {
            MeshFilter filter = planes[i].GetComponent<MeshFilter>();
            meshes[i].vertices = vertices;
            meshes[i].triangles = planeIndicesList[i].ToArray();
            filter.mesh = meshes[i];

            planes[i].transform.position = new Vector3(0, planes[i].transform.position.y, 0);
        }
    }
}
