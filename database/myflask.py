# coding=utf-8
from flask_bootstrap import Bootstrap
from flask_wtf import FlaskForm
from flask import Flask, render_template,flash,request,redirect,url_for
from flask_sqlalchemy import SQLAlchemy
from flask_wtf import FlaskForm
from wtforms import StringField,SubmitField,IntegerField,FloatField,DateTimeField
from wtforms.validators import DataRequired
import os
import sys

# sys.setdefaultencoding('utf-8')

app = Flask(__name__)
basedir = os.path.abspath(os.path.dirname(__file__))
bootstrap = Bootstrap(app)

app.config['SQLALCHEMY_DATABASE_URI'] =\
	'sqlite:///' + os.path.join(basedir, 'data.sqlite')
# app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://root:###@132.232.32.225:3306/dbhomework'
# app.config['SQLALCHEMY_TRACH_MODIFICATIONS'] = False
app.config['SQLALCHEMY_COMMIT_ON_TEARDOWN'] = True
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = True 
app.config['SQLALCHEMY_COMMIT_TEARDOWN'] = True
app.secret_key = 'yishiyu'

db = SQLAlchemy(app)

###############################################
#Customer
###############################################
class Customer(db.Model):
	__tablename__ = 'CUSTOMER'
	c_custkey = db.Column(db.Integer, primary_key=True)
	c_name = db.Column(db.String(25))
	c_address = db.Column(db.String(40))
	c_nationkey = db.Column(db.Integer) #, db.ForeignKey('nation.n_nationkey')
	c_phone = db.Column(db.String(16))
	c_acctbal = db.Column(db.Float)
	c_mktsegment = db.Column(db.String(10))
	c_comment = db.Column(db.String(117))

class CustomerForm(FlaskForm):
	c_custkey = IntegerField(u'客户id', validators=[DataRequired()])
	c_name = StringField(u'姓名', validators=[DataRequired()])
	c_address = StringField(u'地址', validators=[DataRequired()])
	c_phone = StringField(u'电话', validators=[DataRequired()])
	c_comment = StringField(u'评论', validators=[DataRequired()])
	submit = SubmitField(u'添加')

@app.route('/',methods=['GET','POST'])
def index():
    form = CustomerForm()
    if form.validate_on_submit():
        customers=Customer.query.filter_by(c_custkey=form.c_custkey).all()
    else:
        customers = Customer.query.all()
    return render_template('customer.html',customers = customers,form=form)


#删掉顾客
@app.route('/delete_customer/<int:customer_id>')
def delete_customer(customer_id):
    customer = Customer.query.get(customer_id)
    if customer:
        try:
            db.session.delete(customer)
            db.session.commit()
            flash(u'成功删除此顾客')
        except Exception as e:
            print(e)
            flash("删除顾客错误")
            db.session.rollback()


    else:
        flash("顾客找不到")
    print(url_for('index'))
    return redirect(url_for('index'))

# 增加顾客
@app.route('/add_customer', methods=['GET', 'POST'])
def add_customer():
	form = CustomerForm()
	if form.validate_on_submit():
		customer = Customer(c_custkey=form.c_custkey.data,
					c_name=form.c_name.data,c_address=form.c_address.data,
                    c_phone=form.c_phone.data,c_comment=form.c_comment.data)
		db.session.add(customer)
		db.session.commit()
		return redirect(url_for('index'))
	else:
		return render_template('add_customer.html', form=form)

#修改顾客资料
@app.route('/edit_customer/<int:id>', methods=['GET', 'POST'])
def edit_customer(id):
    customer = Customer.query.get(id)
    form = CustomerForm()
    if form.validate_on_submit():
        customer.c_custkey = form.c_custkey.data
        customer.c_name = form.c_name.data
        customer.c_address = form.c_address.data
        customer.c_phone = form.c_phone.data
        customer.c_comment = form.c_comment.data

        db.session.commit()
        flash(u'顾客信息已更改')
        return redirect(url_for('index'))
    else:
	    return render_template('edit_customer.html', form=form)

@app.route('/customer/search/')
def cutomerSearch():
    form = CustomerForm()
    custkey = request.args.get('c_custkey')
    print('收到输入：', custkey)
    # 1.查询数据库存在记录
    if custkey:
        customer = Customer.query.filter_by(c_custkey = custkey)
    else:
        customer = Customer.query.all()
    return render_template('customer.html',customers=customer,form=form)

########################################################
#part
########################################################
class Part(db.Model):
    __tablename__ = 'PART'
    p_partkey = db.Column(db.Integer, primary_key=True)
    p_name = db.Column(db.String(55))
    p_mfgr = db.Column(db.String(25))
    p_brand = db.Column(db.String(10))
    p_type = db.Column(db.String(25))
    p_size = db.Column(db.Integer)
    p_container = db.Column(db.String(10))
    p_retailprice = db.Column(db.Float)
    p_comment = db.Column(db.String(23))


class PartForm(FlaskForm):
    p_partkey = IntegerField(u'零件编号', validators=[DataRequired()])
    p_name = StringField(u'零件名称', validators=[DataRequired()])
    p_mfgr = StringField(u'MFGR', validators=[DataRequired()])
    p_brand = StringField(u'品牌', validators=[DataRequired()])
    p_type = StringField(u'型号', validators=[DataRequired()])
    p_size = IntegerField(u'尺寸', validators=[DataRequired()])
    p_container = StringField(u'包装容器', validators=[DataRequired()])
    p_retailprice = FloatField(u'零售价', validators=[DataRequired()])
    p_comment = StringField(u'备注', validators=[DataRequired()])
    submit = SubmitField(u'添加')

@app.route('/part',methods=['GET','POST'])
def part():
    form = PartForm()
    if form.validate_on_submit():
        parts=Part.query.filter_by(p_partkey=form.p_partkey).all()
    else:
        parts = Part.query.all()
    return render_template('part.html', parts=parts, form=form)

#删掉零件
@app.route('/delete_part/<int:part_id>')
def delete_part(part_id):
    part = Part.query.get(part_id)
    if part:
        try:
            db.session.delete(part)
            db.session.commit()
            flash(u'成功删除此零件')
        except Exception as e:
            print(e)
            flash("删除零件错误")
            db.session.rollback()
    else:
        flash("零件找不到")
    print(url_for('part'))
    return redirect(url_for('part'))

# 增加零件
@app.route('/add_part', methods=['GET', 'POST'])
def add_part():
	form = PartForm()
	if form.validate_on_submit():
		part = Part(p_partkey=form.p_partkey.data,
                            p_name=form.p_name.data,p_mfgr=form.p_mfgr.data,
                            p_brand=form.p_brand.data,p_type=form.p_type.data,
                            p_size=form.p_size.data,p_container=form.p_container.data,
                            p_retailprice=form.p_retailprice.data,p_comment=form.p_comment.data)
		db.session.add(part)
		db.session.commit()
		return redirect(url_for('part'))
	else:
		return render_template('add_part.html', form=form)

#修改零件资料
@app.route('/edit_part/<int:id>', methods=['GET', 'POST'])
def edit_part(id):
    part = Part.query.get(id)
    form = PartForm()
    if form.validate_on_submit():
        part.p_partkey = form.p_partkey.data
        part.p_name = form.p_name.data
        part.p_mfgr = form.p_mfgr.data
        part.p_brand = form.p_brand.data
        part.p_type = form.p_type .data
        part.p_size = form.p_size .data
        part.p_container = form.p_container.data
        part.p_retailprice = form.p_retailprice.data
        part.p_comment = form.p_comment.data
        db.session.commit()
        flash(u'零件信息已更改')
        return redirect(url_for('part'))
    else:
	    return render_template('edit_part.html', form=form)

@app.route('/part/search/')
def partSearch():
    form = PartForm()
    partkey = request.args.get('p_partkey')
    print('收到输入：', partkey)
    # 1.查询数据库存在记录
    if partkey:
        part = Part.query.filter_by(p_partkey = partkey)
    else:
        part = Part.query.all()
    return render_template('part.html',parts=part,form=form)


########################################################
#supplier
########################################################

class Supplier(db.Model):
	__tablename__ = 'SUPPLIER'
	s_suppkey = db.Column(db.Integer, primary_key=True)
	s_name = db.Column(db.String(25))
	s_address = db.Column(db.String(40))
	s_nationkey = db.Column(db.Integer) #, db.ForeignKey('nation.n_nationkey')
	s_phone = db.Column(db.String(15))
	s_acctbal = db.Column(db.Float)
	s_comment = db.Column(db.String(101))

class SupplierForm(FlaskForm):
    s_suppkey = IntegerField(u'供应商编号', validators=[DataRequired()])
    s_name = StringField(u'供应商名称', validators=[DataRequired()])
    s_address = StringField(u'供应商地址', validators=[DataRequired()])
    s_nationkey = IntegerField(u'所属国家', validators=[DataRequired()])
    s_phone = StringField(u'电话', validators=[DataRequired()])
    s_acctbal = FloatField(u'账户余额', validators=[DataRequired()])
    s_comment = StringField(u'备注', validators=[DataRequired()])
    submit = SubmitField(u'添加')

@app.route('/supplier',methods=['GET','POST'])
def supplier():
    form = SupplierForm()
    if form.validate_on_submit():
        suppliers=Supplier.query.filter_by(s_suppkey=form.s_suppkey).all()
    else:
        suppliers = Supplier.query.all()
    return render_template('supplier.html', suppliers = suppliers,form=form)


#删掉供应商
@app.route('/delete_supplier/<int:supplier_id>')
def delete_supplier(supplier_id):
    supplier = Supplier.query.get(supplier_id)
    if supplier:
        try:
            db.session.delete(supplier)
            db.session.commit()
            flash(u'成功删除此供应商')
        except Exception as e:
            print(e)
            flash("删除供应商错误")
            db.session.rollback()


    else:
        flash("供应商找不到")
    print(url_for('supplier'))
    return redirect(url_for('supplier'))

# 增加供应商
@app.route('/add_supplier', methods=['GET', 'POST'])
def add_supplier():
    form = SupplierForm()
    if form.validate_on_submit():
        supplier = Supplier(s_suppkey=form.s_suppkey.data,
                    s_name=form.s_name.data,s_address=form.s_address.data,
                    s_nationkey=form.s_nationkey.data,s_phone=form.s_phone.data,
                    s_acctbal=form.s_acctbal.data,s_comment=form.s_comment.data)
        db.session.add(supplier)
        db.session.commit()
        return redirect(url_for('supplier'))
    else:
        return render_template('add_supplier.html', form=form)

#修改供应商资料
@app.route('/edit_supplier/<int:id>', methods=['GET', 'POST'])
def edit_supplier(id):
    supplier = Supplier.query.get(id)
    form = SupplierForm()
    if form.validate_on_submit():
        supplier.s_suppkey=form.s_suppkey.data
        supplier.s_name=form.s_name.data
        supplier.s_address=form.s_address.data
        supplier.s_nationkey=form.s_nationkey.data
        supplier.s_phone=form.s_phone.data
        supplier.s_acctbal=form.s_acctbal.data
        supplier.s_comment=form.s_comment.data

        db.session.commit()
        flash(u'供应商信息已更改')
        return redirect(url_for('supplier'))
    else:
        return render_template('edit_supplier.html', form=form)

@app.route('/supplier/search/')
def supplierSearch():
    form = SupplierForm()
    suppkey = request.args.get('s_suppkey')
    print('收到输入：', suppkey)
    # 1.查询数据库存在记录
    if suppkey:
        supplier = Supplier.query.filter_by(s_suppkey = suppkey)
    else:
        supplier = Supplier.query.all()
    return render_template('supplier.html',suppliers=supplier,form=form)

###############################################
#partsupp   need to fix multi keys
###############################################
class Partsupp(db.Model):
    __tablename__ = 'PARTSUPP'

    ps_partkey = db.Column(db.Integer, primary_key=True) #, db.ForeignKey('part.partkey')
    ps_suppkey = db.Column(db.Integer, primary_key=True) #, db.ForeignKey('supplier.s_suppkey'),
    ps_availqty = db.Column(db.Integer)
    ps_supplycost = db.Column(db.Float)
    ps_comment = db.Column(db.String(199))

class PartsuppForm(FlaskForm):
    ps_partkey = IntegerField(u'零件编号', validators=[DataRequired()])
    ps_suppkey = IntegerField(u'供应商编号', validators=[DataRequired()])
    ps_availqty = IntegerField(u'供应数量', validators=[DataRequired()])
    ps_supplycost =FloatField(u'供应价格', validators=[DataRequired()])
    ps_comment = StringField(u'备注', validators=[DataRequired()])
    submit = SubmitField(u'添加')

@app.route('/partsupp',methods=['GET','POST'])
def partsupp():
    form = PartsuppForm()
    if form.validate_on_submit():
        partsupps= Partsupp.query.filter_by(ps_partkey=form.ps_partkey,ps_suppkey=form.ps_suppkey).all()
    else:
        partsupps = Partsupp.query.all()
    return render_template('partsupp.html',partsupps = partsupps,form=form)


#删掉零件供应
@app.route('/delete_partsupp/<int:partsupp_id>')
def delete_partsupp(partsupp_id):
    partsupp = Partsupp.query.get(partsupp_id)
    if partsupp:
        try:
            db.session.delete(partsupp)
            db.session.commit()
            flash(u'成功删除此零件供应')
        except Exception as e:
            print(e)
            flash("删除零件供应错误")
            db.session.rollback()


    else:
        flash("零件供应找不到")
    print(url_for('partsupp'))
    return redirect(url_for('partsupp'))

# 增加零件供应
@app.route('/add_partsupp', methods=['GET', 'POST'])
def add_partsupp():
    form = PartsuppForm()
    if form.validate_on_submit():
        partsupp = Partsupp(ps_partkey=form.ps_partkey.data,
                            ps_suppkey=form.ps_suppkey.data,ps_availqty=form.ps_availqty.data,
                            ps_supplycost=form.ps_supplycost.data,ps_comment=form.ps_comment.data)
        db.session.add(partsupp)
        db.session.commit()
        return redirect(url_for('partsupp'))
    else:
        return render_template('add_partsupp.html', form=form)

#修改零件供应资料
@app.route('/edit_partsupp/<int:id>', methods=['GET', 'POST'])
def edit_partsupp(id):
    partsupp = Partsupp.query.get(id)
    form = PartsuppForm()
    if form.validate_on_submit():
        partsupp.ps_partkey=form.ps_partkey.data
        partsupp.ps_suppkey=form.ps_suppkey.data
        partsupp.ps_availqty=form.ps_availqty.data
        partsupp.ps_supplycost=form.ps_supplycost.data
        partsupp.ps_comment=form.ps_comment.data

        db.session.commit()
        flash(u'零件供应信息已更改')
        return redirect(url_for('partsupp'))
    else:
	    return render_template('edit_partsupp.html', form=form)

@app.route('/partsupp/search/')
def partsuppSearch():
    form = PartsuppForm()
    partkey = request.args.get('ps_partkey')
    print('收到输入：', partkey)
    # 1.查询数据库存在记录
    if partkey:
        partsupp = Partsupp.query.filter_by(ps_partkey = partkey)
    else:
        partsupp = Partsupp.query.all()
    return render_template('partsupp.html',partsupps=partsupp,form=form)

###############################################
#Customer
###############################################
class Region(db.Model):
    __tablename__ = 'REGION'
    r_regionkey = db.Column(db.Integer, primary_key=True)
    r_name = db.Column(db.String(25))
    r_comment = db.Column(db.String(152))
    ps_supplycost = db.Column(db.Float)
    ps_comment = db.Column(db.String(199))

class RegionForm(FlaskForm):
    r_regionkey = IntegerField(u'地区代码', validators=[DataRequired()])
    r_name = StringField(u'地区名称', validators=[DataRequired()])
    r_comment = StringField(u'备注', validators=[DataRequired()])
    ps_supplycost = StringField(u'供应价格', validators=[DataRequired()])
    ps_comment = StringField(u'备注', validators=[DataRequired()])
    submit = SubmitField(u'添加')

@app.route('/region',methods=['GET','POST'])
def region():
    form = RegionForm()
    if form.validate_on_submit():
        regions=Region.query.filter_by(r_regionkey=form.r_regionkey).all()
    else:
        regions = Region.query.all()
    return render_template('region.html',regions = regions,form=form)


#删掉地区
@app.route('/delete_region/<int:region_id>')
def delete_region(region_id):
    region = Region.query.get(region_id)
    if region:
        try:
            db.session.delete(region)
            db.session.commit()
            flash(u'成功删除此地区')
        except Exception as e:
            print(e)
            flash("删除地区错误")
            db.session.rollback()
    else:
        flash("地区找不到")
    print(url_for('region'))
    return redirect(url_for('region'))

# 增加地区
@app.route('/add_region', methods=['GET', 'POST'])
def add_region():
    form = RegionForm()
    if form.validate_on_submit():
        region = Region(r_regionkey=form.r_regionkey.data,
                        r_name=form.r_name.data,r_comment=form.r_comment.data,
                        ps_supplycost=form.ps_supplycost.data,ps_comment=form.ps_comment.data)
        db.session.add(region)
        db.session.commit()
        return redirect(url_for('region'))
    else:
        return render_template('add_region.html', form=form)

#修改地区资料
@app.route('/edit_region/<int:id>', methods=['GET', 'POST'])
def edit_region(id):
    region = Region.query.get(id)
    form = RegionForm()
    if form.validate_on_submit():
        region.r_regionkey=form.r_regionkey.data
        region.r_name=form.r_name.data
        region.r_comment=form.r_comment.data
        region.ps_supplycost=form.ps_supplycost.data
        region.ps_comment=form.ps_comment.data
        db.session.commit()
        flash(u'地区信息已更改')
        return redirect(url_for('region'))
    else:
        return render_template('edit_region.html', form=form)

@app.route('/region/search/')
def regionSearch():
    form = RegionForm()
    regionkey = request.args.get('r_regionkey')
    print('收到输入：', regionkey)
    # 1.查询数据库存在记录
    if regionkey:
        region = Region.query.filter_by(r_regionkey = regionkey)
    else:
        region = Region.query.all()
    return render_template('region.html',regions=region,form=form)

###############################################
#Nation
###############################################
class Nation(db.Model):
    __tablename__ = 'NATION'
    n_nationkey = db.Column(db.Integer, primary_key=True)
    n_name = db.Column(db.String(25))
    n_regionkey = db.Column(db.Integer) #db.ForeignKey('region.r_regionkey')
    n_comment = db.Column(db.String(152))

class NationForm(FlaskForm):
    n_nationkey = IntegerField(u'国家编码', validators=[DataRequired()])
    n_name = StringField(u'国家名称', validators=[DataRequired()])
    n_regionkey = StringField(u'所属地区', validators=[DataRequired()])
    n_comment = StringField(u'备注', validators=[DataRequired()])
    submit = SubmitField(u'添加')

@app.route('/nation',methods=['GET','POST'])
def nation():
    form = NationForm()
    if form.validate_on_submit():
        nations= Nation.query.filter_by(n_nationkey=form.n_nationkey).all()
    else:
        nations = Nation.query.all()
    return render_template('nation.html',nations = nations,form=form)


#删掉国家
@app.route('/delete_nation/<int:nation_id>')
def delete_nation(nation_id):
    nation = Nation.query.get(nation_id)
    if nation:
        try:
            db.session.delete(nation)
            db.session.commit()
            flash(u'成功删除此国家')
        except Exception as e:
            print(e)
            flash("删除国家错误")
            db.session.rollback()
    else:
        flash("国家找不到")
    print(url_for('nation'))
    return redirect(url_for('nation'))

# 增加国家
@app.route('/add_nation', methods=['GET', 'POST'])
def add_nation():
    form = NationForm()
    if form.validate_on_submit():
        nation = Nation(n_nationkey=form.n_nationkey.data,
                        n_name=form.n_name.data,n_regionkey=form.n_regionkey.data,
                        n_comment=form.n_comment.data)
        db.session.add(nation)
        db.session.commit()
        return redirect(url_for('nation'))
    else:
        return render_template('add_nation.html', form=form)

#修改国家资料
@app.route('/edit_nation/<int:id>', methods=['GET', 'POST'])
def edit_nation(id):
    nation = Nation.query.get(id)
    form = NationForm()
    if form.validate_on_submit():
        nation.n_nationkey=form.n_nationkey.data
        nation.n_name=form.n_name.data
        nation.n_regionkey=form.n_regionkey.data
        nation.n_comment=form.n_comment.data
        db.session.commit()
        flash(u'国家信息已更改')
        return redirect(url_for('nation'))
    else:
        return render_template('edit_nation.html', form=form)

@app.route('/nation/search/')
def nationSearch():
    form = NationForm()
    nationkey = request.args.get('n_nationkey')
    print('收到输入：', nationkey)
    # 1.查询数据库存在记录
    if nationkey:
        nation = Nation.query.filter_by(n_nationkey = nationkey)
    else:
        nation = Nation.query.all()
    return render_template('nation.html',nations=nation,form=form)

###############################################
#Order  add 和 edit有bug
###############################################
class Order(db.Model):
	__tablename__ = 'ORDER'
	o_orderkey = db.Column(db.Integer, primary_key=True)
	o_custkey = db.Column(db.Integer) #, db.ForeignKey('customer.c_custkey')
	o_orderstatus = db.Column(db.String(1))
	o_totalprice = db.Column(db.Float)
	o_orderdate = db.Column(db.Date)
	o_orderpriority = db.Column(db.String(15))
	o_clerk = db.Column(db.String(16))
	o_shippriority = db.Column(db.Integer)
	o_comment = db.Column(db.String(79))

class OrderForm(FlaskForm):
    o_orderkey = IntegerField(u'订单id', validators=[DataRequired()])
    o_custkey = IntegerField(u'顾客编号', validators=[DataRequired()])
    o_orderstatus= StringField(u'订单状态', validators=[DataRequired()])
    o_totalprice = FloatField(u'订单金额', validators=[DataRequired()])
    o_orderdate = DateTimeField(u'订单日期', validators=[DataRequired()])
    o_orderpriority = StringField(u'优先级', validators=[DataRequired()])
    o_clerk = StringField(u'制单员', validators=[DataRequired()])
    o_shippriority = IntegerField(u'运输优先级', validators=[DataRequired()])
    o_comment = StringField(u'备注', validators=[DataRequired()])

    submit = SubmitField(u'添加')

@app.route('/order',methods=['GET','POST'])
def order():
    form = OrderForm()
    if form.validate_on_submit():
        orders=Order.query.filter_by(o_orderkey=form.o_orderkey).all()
    else:
        orders = Order.query.all()
    return render_template('order.html',orders = orders,form=form)


#删掉订单
@app.route('/delete_order/<int:order_id>')
def delete_order(order_id):
    order = Order.query.get(order_id)
    if order:
        try:
            db.session.delete(order)
            db.session.commit()
            flash(u'成功删除此订单')
        except Exception as e:
            print(e)
            flash("删除订单错误")
            db.session.rollback()
    else:
        flash("订单找不到")
    print(url_for('order'))
    return redirect(url_for('order'))

# 增加订单
@app.route('/add_order', methods=['GET', 'POST'])
def add_order():
    form = OrderForm()
    if form.validate_on_submit():
        order = Order(o_orderkey=form.o_orderkey.data,
                      o_custkey=form.o_custkey.data,o_orderstatus=form.o_orderstatus.data,
                      o_totalprice=form.o_totalprice.data,o_orderdate=form.o_orderdate.data,
                      o_orderpriority=form.o_orderpriority.data,o_clerk=form.o_clerk.data,
                      o_shippriority=form.o_shippriority.data,o_comment =form.o_comment.data)
        db.session.add(order)
        db.session.commit()
        return redirect(url_for('order'))
    else:
         return render_template('add_order.html', form=form)



#修改订单
@app.route('/edit_order/<int:id>', methods=['GET', 'POST'])
def edit_order(id):
    order = Order.query.get(id)
    form = OrderForm()
    if form.validate_on_submit():
        order.o_orderkey=form.o_orderkey.data
        order.o_custkey=form.o_custkey.data
        order.o_orderstatus=form.o_orderstatus.data
        order.o_totalprice=form.o_totalprice.data
        order.o_orderdate=form.o_orderdate.data
        order.o_orderpriority=form.o_orderpriority.data
        order.o_clerk=form.o_clerk.data
        order.o_shippriority=form.o_shippriority.data
        order.o_comment =form.o_comment.data
        db.session.commit()
        flash(u'订单信息已更改')
        return redirect(url_for('order'))
    else:
        return render_template('edit_order.html', form=form)

@app.route('/order/search/')
def orderSearch():
    form = OrderForm()
    orderkey = request.args.get('o_orderkey')
    print('收到输入：', orderkey)
    # 1.查询数据库存在记录
    if orderkey:
        order = Order.query.filter_by(o_orderkey = orderkey)
    else:
        order = Order.query.all()
    return render_template('order.html',orders=order,form=form)


if __name__ == '__main__':

    app.run(debug=True)
