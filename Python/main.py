from problem import *

if __name__ == "__main__":

    st.markdown(
        """
        <style>
        div[class*="stSlider"] > label > div[data-testid="stMarkdownContainer"] > p 
        {
        font-size: 20px;
        }
        div[class*="stRadio"] > label > div[data-testid="stMarkdownContainer"] > p 
        {
        font-size: 20px;
        }
        div[class*="stTextInput"] > label > div[data-testid="stMarkdownContainer"] > p 
        {
        font-size: 20px;
        }
        div[class*="stTextArea"] > label > div[data-testid="stMarkdownContainer"] > p 
        {
        font-size: 20px;
        }
        </style>
        """, unsafe_allow_html=True)
    
    st.title("Linear Programming")
    st.subheader("Đây là chương trình của nhóm chúng mình xây dựng để giải bài toán quy hoạch tuyến tính.")

    st.markdown("---")
    st.markdown("## Thông tin nhóm")
    team = pd.DataFrame({"Họ và Tên": ["Võ Thị Khánh Linh", "Nguyễn Nhật Minh Thư"], "MSSV": ["21280070", "21280112"], "Lớp": ["21KDL1", "21KDL1"], "Nhóm trưởng": ["x",""]})
    team.index = [1,2]
    st.table(team)

    st.markdown("---")
    st.markdown("## Bài toán")

    st.subheader("Nhập số biến")
    numVariables = st.slider('Số biến là ?', 0, 20, 3, key = "numVar")

    st.subheader("Nhập số ràng buộc")
    numConstraints = st.slider('Số ràng buộc là ?', 0, 20, 3, key = "numCon")

    st.subheader("Nhập hàm mục tiêu")
    Min = st.radio("Mục tiêu tối ưu là ?",('Min', 'Max'))
    if Min == "Min":
        isMin = True
    else:
        isMin = False
    
    Target = st.text_input('Nhập biểu thức tối ưu', placeholder= "VD: 3x1 + 4x2 - x3")

    st.subheader("Nhập các ràng buộc \(bất\) đẳng thức")
    Constraints = st.text_area(label = "Ràng buộc", height = 150, placeholder= "VD:\n  3x1 + 4x2 - x3 >= -5\n- 2x1 - x2 + 6x3 = 3\n- x1 + 3x2 - 4x3 <= -6")

    st.subheader("Nhập các ràng buộc về dấu")
    Variables = st.text_area(label = "Ràng buộc về dấu", height = 150, placeholder= "VD:\nx1 >= 0\nx2 tự do\nx3 <= 0")

    if st.button('GIẢI BÀI TOÁN'):
        num_variables, num_constraints, is_min, target, A, b, sign_constraints, sign_variables = preprocessing_problem(numVariables, numConstraints, isMin, Target, Constraints, Variables)
        p = Problem(num_variables, num_constraints, is_min, target, A, b, sign_constraints, sign_variables)
        p.solve_problem()
        
